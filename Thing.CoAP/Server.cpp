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

		void Server::AddResource(Thing::CoAP::IResource* resource)
		{
			AvailableResource r;
			r.resource = resource;
			r.observers = std::list<Thing::CoAP::Observer>();
			resources[resource->GetName()] = r;
			resource->SetServer(this);
		}

		void Server::AddResource(Thing::CoAP::IResource& endpoint)
		{
			AddResource(&endpoint);
		}

		void Server::RemoveResource(Thing::CoAP::IResource* resource)
		{
			resources.erase(resource->GetName());
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
				response.SetVersion(request.GetVersion());
				response.SetTokens(request.GetTokens());
				response.SetMessageID(request.GetMessageID());
				switch (request.GetCode())
				{
				case Thing::CoAP::Method::Empty:
					if (request.GetType() == Thing::CoAP::MessageType::Confirmable)
					{
						response.SetType(Thing::CoAP::MessageType::Reset);
						response.SetCode(Thing::CoAP::Method::Empty);
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
					switch (request.GetType())
					{
					case Thing::CoAP::MessageType::Confirmable: response.SetType(Thing::CoAP::MessageType::Acknowledge); break;
					case Thing::CoAP::MessageType::NonConfirmable: response.SetType(Thing::CoAP::MessageType::NonConfirmable); break;
					}

					std::map<std::string, AvailableResource>::iterator it = this->resources.find(url);
					if (it == resources.end())
					{
						if (request.GetCode() == Thing::CoAP::Method::Get && url == ".well-known/core")
							resourceDiscovery(&response, address, port);
						else
							noEndpointDefinedResponse(&response, address, port);
					}
					else
					{
						Thing::CoAP::IResource* resource = it->second.resource;
						Thing::CoAP::Status e = Thing::CoAP::Status::Content();
						switch (request.GetCode())
						{
						case Thing::CoAP::Method::Get:
						{
							bool observeRequestButEndpointDoesntSupport = false;
							for (Thing::CoAP::Option& option : request.GetOptions())
								if (option.GetNumber() == Thing::CoAP::OptionValue::Observe)
								{
									if (!resource->IsObservable())
									{
										observeRequestButEndpointDoesntSupport = true;
										break;
									}

									Thing::CoAP::Observer obs(address, port, Thing::CoAP::Functions::GenerateMessageID(), request.GetTokens());
									removeObserver(url, obs);
									if (option.GetLenght() == 0)
									{
										AddObserveOption(response, obs);
										addObserver(url, obs);
									}
									break;
								}

							e = observeRequestButEndpointDoesntSupport ? Status::MethodNotAllowed() : resource->Get(request);
							break;
						}
						case Thing::CoAP::Method::Put: e = resource->Put(request); break;
						case Thing::CoAP::Method::Post: e = resource->Post(request); break;
						case Thing::CoAP::Method::Delete: e = resource->Delete(request); break;
						}

						std::string statusPayload = e.GetPayload();
						response.SetCode(static_cast<uint8_t>(e.GetCode()));
						response.SetPayload((uint8_t*)statusPayload.c_str(), static_cast<int>(statusPayload.size()));
						AddContentFormat(response, resource->GetContentFormat());
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

			std::map<std::string, AvailableResource>::iterator observers = this->resources.find(endpointPath);
			if (observers == this->resources.end())
				return; //There are no observers, no point in continue and waste processing.

			Thing::CoAP::Response response;
			response.SetVersion(1);
			response.SetType(Thing::CoAP::MessageType::Confirmable);
			response.SetCode(static_cast<uint8_t>(Thing::CoAP::ResponseCode::Content));
			std::string buffer = r.GetPayload();
			response.SetPayload((uint8_t*)buffer.c_str(), static_cast<int>(buffer.size()));

			for (Thing::CoAP::Observer& obs : observers->second.observers)
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
			resources[url].observers.push_back(obs);
		}

		void Server::removeObserver(std::string& url, Thing::CoAP::Observer & obs)
		{
			if (url.size() > 0)
			{
				std::map<std::string, AvailableResource>::iterator observers = this->resources.find(url);
				if (observers == this->resources.end())
					return;

				observers->second.observers.remove(obs);
				return;
			}

			for (auto it = this->resources.begin(); it != this->resources.end(); ++it)
				it->second.observers.remove(obs);
		}

		void Server::resourceDiscovery(Thing::CoAP::Response* response, IPAddress ip, int port)
		{
			std::string result = "";

			int i = 0;
			for (auto& r : resources)
			{
				Thing::CoAP::IResource* resource = r.second.resource;
				if (i != 0)
					result += ",";

				char ct[3];
				sprintf(ct, "%d", static_cast<int>(resource->GetContentFormat()));

				std::string resourceType = resource->GetResourceType();
				std::string interfaceDescription = resource->GetInterfaceDescription();
				std::string title = resource->GetTitle();
				size_t maximumSizeEstimate = resource->GetMaximumSizeEstimate();

				result += "</" + r.first + ">";

				if(interfaceDescription != "")
					result += ";if=\"" + interfaceDescription + "\"";
				
				if(resourceType != "")
					result += ";rt=\"" + resourceType + "\"";

				if (resource->IsObservable())
					result += ";obs";

				if(title != "")
					result += ";title=\"" + title + "\"";

				if (maximumSizeEstimate != 0)
				{
					char buffer[20];
					sprintf(buffer, ";sz=%zu", maximumSizeEstimate);
					result += std::string(buffer);
				}
				
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