#pragma once

#include "IServer.h"

namespace Thing {
	namespace CoAP
	{
		class IPacketProvider
		{
		public:
			virtual void Start(int port) = 0;
			virtual void Stop() = 0;

			virtual void SendPacket(std::vector<uint8_t> buffer, Thing::CoAP::IPAddress ip, int port) = 0;
			virtual bool ReadPacket(std::vector<uint8_t>* buffer, Thing::CoAP::IPAddress* address, int* port) = 0;
		};
	}
}