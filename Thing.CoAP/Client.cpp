#include "Client.h"

namespace Thing
{
	namespace CoAP
	{
		Client::Client() : packetProvider(NULL), currentMessageId(0)
		{
		}

		Client::~Client()
		{

		}

		void Client::SetPacketProvider(IPacketProvider* packetProvider)
		{
			this->packetProvider = packetProvider;
			this->packetProvider->Start(1803);
		}

		void Client::SetPacketProvider(IPacketProvider& packetProvider)
		{
			SetPacketProvider(&packetProvider);
		}

		void Client::Request(IPAddress ip, int port, std::string endpoint, Method method, std::vector<uint8_t> payload, ResponseCallback callback)
		{
			if (packetProvider == NULL)
				return;

			const uint16_t messageID = currentMessageId++;
			uint8_t tokens[2] = { (messageID >> 8) & 0xFF, messageID & 0xFF };

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

			uint8_t* requestBuffer;
			int requestLength;
			request.SerializePacket(&requestBuffer, &requestLength);

			packetProvider->SendPacket(requestBuffer, requestLength, ip, port);
			pendingRequests[messageID] = callback;

			delete[] requestBuffer;
		}

		void Client::Request(IPAddress ip, int port, std::string endpoint, Method method, std::string payload, ResponseCallback callback)
		{
			std::vector<uint8_t> bytes((uint8_t*)payload.c_str(), (uint8_t*)payload.c_str() + payload.size());
			Request(ip, port, endpoint, method, bytes, callback);
		}

		void Client::Process()
		{
			if (!packetProvider)
				return;

			uint8_t* buffer = NULL;
			int packetLength = 0;

			IPAddress address;
			int port = 0;
			if (packetProvider->ReadPacket(&buffer, &packetLength, &address, &port))
			{
				Thing::CoAP::Response response;
				response.DesserializePacket(buffer, packetLength);

				std::vector<uint8_t> tokens = response.GetTokens();
				uint16_t messageID = ((tokens[0] << 8) | tokens[1]);
				auto entry = pendingRequests.find(messageID);
				if (entry != pendingRequests.end())
					entry->second(response);

				packetProvider->DestroyPacket(buffer);
			}
		}
	}
}