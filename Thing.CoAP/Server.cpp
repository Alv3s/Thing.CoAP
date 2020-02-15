#include "Server.h"
#include "ContentFormat.h"
#include "OptionValue.h"
#include <stdio.h>
#include "Functions.h"
#include "FunctionalResource.h"

namespace Thing {
	namespace CoAP
	{
		static void AddContentFormat(Thing::CoAP::Response& response, Thing::CoAP::ContentFormat format)
		{
			uint8_t optionBuffer[2];
			optionBuffer[0] = (static_cast<uint16_t>(format) & 0xFF00) >> 8;
			optionBuffer[1] = (static_cast<uint16_t>(format) & 0x00FF);

			std::vector<Thing::CoAP::Option>& options = response.GetOptions();
			Thing::CoAP::Option option;
			option.SetNumber(Thing::CoAP::OptionValue::ContentFormat);
			option.SetOption(optionBuffer, 2);
			options.push_back(option);
		}

		static void AddObserveOption(Thing::CoAP::Response& response, Thing::CoAP::Observer& obs)
		{
			uint16_t obsCount = obs.NextCount();
			uint8_t optionBuffer[2];
			optionBuffer[0] = (static_cast<uint16_t>(obsCount) & 0xFF00) >> 8;
			optionBuffer[1] = (static_cast<uint16_t>(obsCount) & 0x00FF);

			std::vector<Thing::CoAP::Option>& options = response.GetOptions();

			Thing::CoAP::Option observeOption;
			observeOption.SetNumber(Thing::CoAP::OptionValue::Observe);
			observeOption.SetOption(optionBuffer, 2);
			options.push_back(observeOption);
		}

		static void AddURIOption(Thing::CoAP::Response& response, std::string name)
		{
			std::vector<Thing::CoAP::Option>& options = response.GetOptions();

			Thing::CoAP::Option uriOption;
			uriOption.SetOption((uint8_t*)name.c_str(), static_cast<int>(name.size()));
			uriOption.SetNumber(Thing::CoAP::OptionValue::URIPath);
			options.push_back(uriOption);
		}

		Server::Server() : 
			packetProvider(NULL),
			port(5683)
		{
		}

		Server::~Server()
		{
		}

#pragma region Endpoint Management
		IFunctionalResource& Server::CreateResource(std::string name, Thing::CoAP::ContentFormat content, bool IsObservable)
		{
			FunctionalResource* endpoint = new FunctionalResource(name, content, IsObservable);
			AddResource(endpoint);
			return *endpoint;
		}

		void Server::AddResource(Thing::CoAP::IResource* endpoint)
		{
			endpoints[endpoint->GetName()] = endpoint;
			endpoint->SetServer(this);
		}

		void Server::AddResource(Thing::CoAP::IResource& endpoint)
		{
			AddResource(&endpoint);
		}

		void Server::RemoveResource(Thing::CoAP::IResource* endpoint)
		{
			endpoints.erase(endpoint->GetName());
		}

		void Server::RemoveResource(Thing::CoAP::IResource& endpoint)
		{
			RemoveResource(&endpoint);
		}
#pragma endregion

		void Server::SetPort(int port)
		{
			this->port = port;
		}

		int Server::GetPort()
		{
			return this->port;
		}

		void Server::Start()
		{
			if (packetProvider)
				packetProvider->Start(port);
		}

		void Server::Stop()
		{
			if (packetProvider)
				packetProvider->Stop();
		}

		void Server::Process()
		{
			if (!packetProvider)
				return;

			IPAddress address;
			int port = 0;
			std::vector<uint8_t> buffer;
			if (packetProvider->ReadPacket(&buffer, &address, &port))
			{
				Thing::CoAP::Request request;
				request.DesserializePacket(buffer);

				//call endpoint url function
				std::string url = "";
				std::vector<Thing::CoAP::Option>& options = request.GetOptions();
				for (Thing::CoAP::Option& option : options)
					if (option.GetNumber() == Thing::CoAP::OptionValue::URIPath && option.GetLenght() > 0)
					{
						std::string urlname((char*)option.GetBuffer(), (int)option.GetLenght());
						if (url.length() > 0)
							url += "/";
						url += urlname;
					}

				//response
				Thing::CoAP::Response response;
				switch (request.GetCode())
				{
				case Thing::CoAP::Method::Empty:
					if (request.GetType() == Thing::CoAP::MessageType::Confirmable)
					{
						response.SetVersion(request.GetVersion());
						response.SetType(Thing::CoAP::MessageType::Reset);
						response.SetCode(Thing::CoAP::Method::Empty);
						response.SetMessageID(request.GetMessageID());
						response.SetTokens(request.GetTokens());
						response.SetPayload(NULL, 0);
					}
					else if (request.GetType() == Thing::CoAP::MessageType::Reset)
					{
						Thing::CoAP::Observer observer(address, port, Thing::CoAP::Functions::GenerateMessageID());
						removeObserver(url, observer);
					}
					break;
				case Thing::CoAP::Method::Get: case Thing::CoAP::Method::Post: case Thing::CoAP::Method::Put: case Thing::CoAP::Method::Delete:
				{
					response.SetVersion(request.GetVersion());
					response.SetTokens(request.GetTokens());
					response.SetMessageID(request.GetMessageID());
					switch (request.GetType())
					{
					case Thing::CoAP::MessageType::Confirmable: response.SetType(Thing::CoAP::MessageType::Acknowledge); break;
					case Thing::CoAP::MessageType::NonConfirmable: response.SetType(Thing::CoAP::MessageType::NonConfirmable); break;
					}

					switch (request.GetCode())
					{
					case Thing::CoAP::Method::Get:
					{
						if (url == ".well-known/core")
						{
							resourceDiscovery(&response, address, port);
							break;
						}

						if (endpoints.find(url) == endpoints.end())
						{
							noEndpointDefinedResponse(&response, address, port);
							break;
						}

						Thing::CoAP::IResource* endpoint = endpoints[url];
						bool observeRequestButEndpointDoesntSupport = false;
						for (Thing::CoAP::Option& option : request.GetOptions())
							if (option.GetNumber() == Thing::CoAP::OptionValue::Observe)
							{
								if (!endpoint->IsObservable())
								{
									observeRequestButEndpointDoesntSupport = true;
									break;
								}

								Thing::CoAP::Observer obs(address, port, Thing::CoAP::Functions::GenerateMessageID(), request.GetTokens());
								if (option.GetLenght() > 0 && option.GetBuffer()[0] == 1)
									removeObserver(url, obs);
								else
								{
									AddObserveOption(response, obs);
									addObserver(url, obs);
								}
								break;
							}
						
						Thing::CoAP::Status e = observeRequestButEndpointDoesntSupport ? Status::MethodNotAllowed() : endpoint->Get(request);
						AddContentFormat(response, endpoint->GetContentFormat());
						std::string payload = e.GetPayload();
						response.SetCode(static_cast<uint8_t>(e.GetCode()));
						response.SetPayload((uint8_t*)payload.c_str(), static_cast<int>(payload.size()));
						break;
					}
					case Thing::CoAP::Method::Put:
					{
						if (endpoints.find(url) == endpoints.end())
						{
							noEndpointDefinedResponse(&response, address, port);
							break;
						}

						Thing::CoAP::IResource* endpoint = endpoints[url];
						Thing::CoAP::Status e = endpoint->Put(request);
						AddContentFormat(response, endpoint->GetContentFormat());
						std::string payload = e.GetPayload();
						response.SetCode(static_cast<uint8_t>(e.GetCode()));
						response.SetPayload((uint8_t*)payload.c_str(), static_cast<int>(payload.size()));
						break;
					}
					case Thing::CoAP::Method::Post:
					{
						if (endpoints.find(url) == endpoints.end())
						{
							noEndpointDefinedResponse(&response, address, port);
							break;
						}

						Thing::CoAP::IResource* endpoint = endpoints[url];
						Thing::CoAP::Status e = endpoint->Post(request);
						AddContentFormat(response, endpoint->GetContentFormat());
						std::string payload = e.GetPayload();
						response.SetCode(static_cast<uint8_t>(e.GetCode()));
						response.SetPayload((uint8_t*)payload.c_str(), static_cast<int>(payload.size()));
						break;
					}
					case Thing::CoAP::Method::Delete:
					{
						if (endpoints.find(url) == endpoints.end())
						{
							noEndpointDefinedResponse(&response, address, port);
							break;
						}

						Thing::CoAP::IResource* endpoint = endpoints[url];
						Thing::CoAP::Status e = endpoint->Delete(request);
						AddContentFormat(response, endpoint->GetContentFormat());
						std::string payload = e.GetPayload();
						response.SetCode(static_cast<uint8_t>(e.GetCode()));
						response.SetPayload((uint8_t*)payload.c_str(), static_cast<int>(payload.size()));
						break;
					}
					}

					std::vector<uint8_t> payload = response.SerializePacket();
					packetProvider->SendPacket(payload, address, port);
				}
				default:
					break;
				}
			}
		}

		void Server::NotifyObservers(Thing::CoAP::IResource* endpoint, Thing::CoAP::Status r)
		{
			const std::string endpointPath = endpoint->GetName();

			Thing::CoAP::Response response;
			response.SetVersion(1);
			response.SetType(Thing::CoAP::MessageType::Confirmable);
			response.SetCode(static_cast<uint8_t>(Thing::CoAP::ResponseCode::Content));
			std::string buffer = r.GetPayload();
			response.SetPayload((uint8_t*)buffer.c_str(), static_cast<int>(buffer.size()));

			for (Thing::CoAP::Observer& obs : observers[endpointPath])
			{
				response.SetMessageID(obs.NextMessageID());

				response.SetOptions(std::vector<Thing::CoAP::Option>());
				AddObserveOption(response, obs);
				AddURIOption(response, endpointPath);
				AddContentFormat(response, endpoint->GetContentFormat());

				response.SetTokens(obs.GetTokens());

				std::vector<uint8_t> payload = response.SerializePacket();
				packetProvider->SendPacket(payload, obs.GetIPAddress(), obs.GetPort());
			}
		}

		void Server::NotifyObservers(Thing::CoAP::IResource& endpoint, Thing::CoAP::Status r)
		{
			NotifyObservers(&endpoint, r);
		}

		void Server::SetPacketProvider(IPacketProvider * provider)
		{
			this->packetProvider = provider;
		}

		void Server::SetPacketProvider(IPacketProvider & provider)
		{
			SetPacketProvider(&provider);
		}

#pragma region Private Methods
		void Server::addObserver(std::string& url, Thing::CoAP::Observer & obs)
		{
			removeObserver(url, obs);
			observers[url].push_back(obs);
		}

		void Server::removeObserver(std::string& url, Thing::CoAP::Observer & obs)
		{
			observers[url].remove(obs);
		}

		void Server::resourceDiscovery(Thing::CoAP::Response* response, IPAddress ip, int port)
		{
			std::string result = "";

			int i = 0;
			for (auto& e : endpoints)
			{
				if (i != 0)
					result += ",";

				char ct[3];
				sprintf(ct, "%d", static_cast<int>(e.second->GetContentFormat()));

				result += "</" + e.first + ">";
				result += ";if=\"" + e.first + "\"";
				if (e.second->IsObservable())
					result += ";obs";
				result += ";ct=" + std::string(ct);
				++i;
			}
			response->ClearOptions();
			AddContentFormat(*response, Thing::CoAP::ContentFormat::ApplicationLinkFormat);
			response->SetCode(Thing::CoAP::Method::Content);
			response->SetPayload((uint8_t*)result.c_str(), static_cast<int>(result.size()));
		}

		void Server::noEndpointDefinedResponse(Thing::CoAP::Response * response, IPAddress ip, int port)
		{
			response->SetPayload(NULL, 0);
			response->SetCode(Thing::CoAP::Method::NotFound);

			uint8_t optionBuffer[2];
			optionBuffer[0] = ((uint16_t)Thing::CoAP::ContentFormat::TextPlain & 0xFF00) >> 8;
			optionBuffer[1] = ((uint16_t)Thing::CoAP::ContentFormat::TextPlain & 0x00FF);

			std::vector<Thing::CoAP::Option> options;
			Thing::CoAP::Option option;
			option.SetNumber(Thing::CoAP::OptionValue::ContentFormat);
			option.SetOption(optionBuffer, 2);
		}
#pragma endregion
	}
}