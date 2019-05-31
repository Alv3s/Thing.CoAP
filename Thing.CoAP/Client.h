#pragma once

#include "Response.h"
#include <vector>
#include <map>
#include <functional>
#include "Status.h"
#include "IPacketProvider.h"
#include "IPAddress.h"

namespace Thing
{
	namespace CoAP
	{
		using ResponseCallback = std::function<void(Thing::CoAP::Response& response)>;

		class Client
		{
		public:
			Client();
			~Client();

			void SetPacketProvider(IPacketProvider* packetProvider);
			void SetPacketProvider(IPacketProvider& packetProvider);

			void Request(IPAddress ip, int port, std::string endpoint, Method method, std::vector<uint8_t> payload, ResponseCallback callback);
			void Request(IPAddress ip, int port, std::string endpoint, Method method, std::string payload, ResponseCallback callback);

			void Process();
		private:
			IPacketProvider* packetProvider;
			std::map<uint16_t, ResponseCallback> pendingRequests;
			uint16_t currentMessageId;
		};
	}
}

