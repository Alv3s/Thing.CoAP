#include "Observer.h"
#include "Observer.h"

namespace Thing {
	namespace CoAP
	{
		Observer::Observer(IPAddress ip, int port, std::vector<uint8_t> tokens) : ip(ip), port(port), tokens(tokens)
		{
		}

		Observer::Observer(IPAddress ip, int port) : ip(ip), port(port), tokens()
		{
		}

		Observer::~Observer()
		{
		}

		IPAddress Observer::GetIPAddress() const
		{
			return ip;
		}

		int Observer::GetPort() const
		{
			return port;
		}

		std::vector<uint8_t> Observer::GetTokens() const
		{
			return tokens;
		}
	}
}