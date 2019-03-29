#pragma once

#include "IServer.h"
#include <string>

namespace Thing {
	namespace CoAP
	{
		class Observer
		{
		public:
			Observer(IPAddress ip, int port, std::vector<uint8_t> tokens);
			Observer(IPAddress ip, int port);
			virtual ~Observer();

			IPAddress GetIPAddress() const;
			int GetPort() const;
			std::vector<uint8_t> GetTokens() const;

			bool operator==(const Observer& other) const
			{
				return ip == other.ip && port == other.port;
			}
		private:
			const IPAddress ip;
			const int port;
			const std::vector<uint8_t> tokens;
		};
	}
}