#include "Client.h"

namespace Thing
{
	namespace CoAP
	{
		Client::Client() : packetProvider(NULL),
			currentMessageId(1),
			peerIP(),
			peerPort(),
			port(5682)
		{
		}

		Client::~Client()
		{

		}

		void Client::SetPort(int port)
		{
			this->port = port;
		}

		int Client::GetPort()
		{
			return port;
		}

		void Client::Start(IPAddress peerIP, int peerPort)
		{
			this->peerIP = peerIP;
			this->peerPort = peerPort;

			if (packetProvider)
				packetProvider->Start(port);
		}

		void Client::Stop()
		{
			if (packetProvider)
				packetProvider->Stop();
		}


		void Client::SetPacketProvider(IPacketProvider* packetProvider)
		{
			this->packetProvider = packetProvider;
		}

		void Client::SetPacketProvider(IPacketProvider& packetProvider)
		{
			SetPacketProvider(&packetProvider);
		}

		void Client::Discover(ResponseCallback callback)
		{
			Get(".well-known/core", callback);
		}

		void Client::Get(std::string endpoint, ResponseCallback callback)
		{
			Request(endpoint, Thing::CoAP::Method::Get, std::vector<uint8_t>(), callback);
		}

		void Client::Get(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback)
		{
			Request(endpoint, Thing::CoAP::Method::Get, payload, callback);
		}

		void Client::Get(std::string endpoint, std::string payload, ResponseCallback callback)
		{
			Request(endpoint, Thing::CoAP::Method::Get, payload, callback);
		}

		void Client::Post(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback)
		{
			Request(endpoint, Thing::CoAP::Method::Post, payload, callback);
		}

		void Client::Post(std::string endpoint, std::string payload, ResponseCallback callback)
		{
			Request(endpoint, Thing::CoAP::Method::Post, payload, callback);
		}

		void Client::Put(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback)
		{
			Request(endpoint, Thing::CoAP::Method::Put, payload, callback);
		}

		void Client::Put(std::string endpoint, std::string payload, ResponseCallback callback)
		{
			Request(endpoint, Thing::CoAP::Method::Put, payload, callback);
		}

		void Client::Delete(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback)
		{
			Request(endpoint, Thing::CoAP::Method::Delete, payload, callback);
		}

		void Client::Delete(std::string endpoint, std::string payload, ResponseCallback callback)
		{
			Request(endpoint, Thing::CoAP::Method::Delete, payload, callback);
		}

		ObserveToken Client::Observe(std::string endpoint, ResponseCallback callback)
		{
			const uint16_t messageID = currentMessageId++;
			uint8_t tokens[2] = { static_cast<uint8_t>((messageID >> 8) & 0xFF),
			                      static_cast<uint8_t>(messageID & (uint8_t)0xFF) };

			Thing::CoAP::Packet request;
			request.SetVersion(1);
			request.SetMessageID(messageID);
			request.SetTokens(std::vector<uint8_t>(tokens, tokens + 2));
			request.SetType(Thing::CoAP::MessageType::NonConfirmable);
			std::vector<Thing::CoAP::Option> options;
			Thing::CoAP::Option observeOption;
			observeOption.SetNumber(Thing::CoAP::OptionValue::Observe);
			options.push_back(observeOption);
			Thing::CoAP::Option urlPath;
			urlPath.SetNumber(Thing::CoAP::OptionValue::URIPath);
			urlPath.SetOption((uint8_t*)endpoint.c_str(), endpoint.size());
			options.push_back(urlPath);
			request.SetOptions(options);
			request.SetCode(Thing::CoAP::Method::Get);

			std::vector<uint8_t> serializedBuffer = request.SerializePacket();

			packetProvider->SendPacket(serializedBuffer, peerIP, peerPort);
			ObserveToken observeToken(messageID, endpoint, callback);
			observeRequests[messageID] = observeToken;

			return observeToken;
		}

		void Client::CancelObserve(ObserveToken token)
		{
			const uint16_t messageID = currentMessageId++;
			const uint16_t tokenID = token.GetToken();
			std::string endpoint = token.GetEndpoint();
			uint8_t tokens[2] = { static_cast<uint8_t>((tokenID >> 8) & 0xFF), static_cast<uint8_t>(tokenID & 0xFF) };

			Thing::CoAP::Packet request;
			request.SetVersion(1);
			request.SetMessageID(messageID);
			request.SetTokens(std::vector<uint8_t>(tokens, tokens + 2));
			request.SetType(Thing::CoAP::MessageType::NonConfirmable);
			std::vector<Thing::CoAP::Option> options;
			Thing::CoAP::Option observeOption;
			observeOption.SetNumber(Thing::CoAP::OptionValue::Observe);
			uint8_t buffer[] = { 1 };
			observeOption.SetOption(buffer, sizeof(buffer));
			options.push_back(observeOption);
			Thing::CoAP::Option urlPath;
			urlPath.SetNumber(Thing::CoAP::OptionValue::URIPath);
			urlPath.SetOption((uint8_t*)endpoint.c_str(), endpoint.size());
			options.push_back(urlPath);
			request.SetOptions(options);
			request.SetCode(Thing::CoAP::Method::Get);

			std::vector<uint8_t> requestBuffer = request.SerializePacket();

			packetProvider->SendPacket(requestBuffer, peerIP, peerPort);

			pendingRequests[tokenID] = [this, &tokenID](Thing::CoAP::Response response)
			{
				observeRequests.erase(tokenID);
			};
		}

		void Client::Process()
		{
			if (!packetProvider)
				return;

			IPAddress address;
			int port = 0;
			std::vector<uint8_t> buffer;
			if (packetProvider->ReadPacket(&buffer, &address, &port))
			{
				Thing::CoAP::Response response;
				response.DesserializePacket(buffer);

				std::vector<uint8_t> tokens = response.GetTokens();
				if (tokens.size() >= 2)
				{
					uint16_t messageID = ((tokens[0] << 8) | tokens[1]);
					auto entryPending = pendingRequests.find(messageID);
					if (entryPending != pendingRequests.end())
					{
						entryPending->second(response);
						pendingRequests.erase(entryPending);
					}
					else
					{
						auto entryObserving = observeRequests.find(messageID);
						if (entryObserving != observeRequests.end())
							entryObserving->second.GetCallback()(response);
					}
				}
			}
		}

		void Client::Request(std::string endpoint, Method method, std::vector<uint8_t> payload, ResponseCallback callback)
		{
			if (packetProvider == NULL)
				return;

			const uint16_t messageID = currentMessageId++;
			uint8_t tokens[2] = { static_cast<uint8_t>((messageID >> 8) & 0xFF), static_cast<uint8_t>(messageID & 0xFF) };

			Thing::CoAP::Packet request;
			request.SetVersion(1);
			request.SetMessageID(messageID);
			request.SetTokens(std::vector<uint8_t>(tokens, tokens + 2));
			request.SetType(Thing::CoAP::MessageType::NonConfirmable);
			std::vector<Thing::CoAP::Option> options;
			Thing::CoAP::Option urlPath;
			urlPath.SetNumber(Thing::CoAP::OptionValue::URIPath);
			urlPath.SetOption((uint8_t*)endpoint.c_str(), endpoint.size());
			options.push_back(urlPath);
			request.SetOptions(options);
			request.SetCode(method);
			if (payload.size() > 0)
				request.SetPayload(&payload[0], payload.size());

			std::vector<uint8_t> requestBuffer = request.SerializePacket();

			packetProvider->SendPacket(requestBuffer, peerIP, peerPort);
			pendingRequests[messageID] = callback;
		}

		void Client::Request(std::string endpoint, Method method, std::string payload, ResponseCallback callback)
		{
			std::vector<uint8_t> bytes((uint8_t*)payload.c_str(), (uint8_t*)payload.c_str() + payload.size());
			Request(endpoint, method, bytes, callback);
		}
	}
}