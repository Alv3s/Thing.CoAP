#pragma once

#include "gmock/gmock.h"
#include "../Thing.CoAP/IPacketProvider.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class PacketProviderMock : public Thing::CoAP::IPacketProvider
			{
			public:
				MOCK_METHOD1(Start, void(int));
				MOCK_METHOD0(Stop, void());

				MOCK_METHOD4(SendPacket, void(uint8_t*, int, Thing::CoAP::IPAddress, int));

				MOCK_METHOD4(ReadPacket, bool(uint8_t**, int*, Thing::CoAP::IPAddress*, int*));
				MOCK_METHOD1(DestroyPacket, void(uint8_t*));
			};
		}
	}
}