#include "Observer.h"
#include "Observer.h"

namespace Thing {
	namespace CoAP
	{
		Observer::Observer(IPAddress ip, int port, uint16_t messageID, std::vector<uint8_t> tokens) :
			ip(ip), 
			port(port), 
			messageID(messageID),
			tokens(tokens), 
			count(1)
		{
		}

		Observer::Observer(IPAddress ip, int port, uint16_t messageID) : 
			ip(ip), 
			port(port), 
			messageID(messageID),
			tokens(),
			count(1)
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

		uint16_t Observer::NextCount()
		{
			return count++;
		}

		uint16_t Observer::NextMessageID()
		{
			return messageID++;
		}

		std::vector<uint8_t> Observer::GetTokens() const
		{
			return tokens;
		}
	}
}