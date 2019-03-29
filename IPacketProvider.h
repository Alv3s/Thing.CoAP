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

			virtual void SendPacket(uint8_t* buffer, int length, Thing::CoAP::IPAddress ip, int port) = 0;

			virtual bool ReadPacket(uint8_t** buffer, int* length, Thing::CoAP::IPAddress* address, int* port) = 0;
			virtual void DestroyPacket(uint8_t* buffer) = 0;
		};
	}
}