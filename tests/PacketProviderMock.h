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

				MOCK_METHOD3(SendPacket, void(std::vector<uint8_t>, Thing::CoAP::IPAddress, int));
				MOCK_METHOD3(ReadPacket, bool(std::vector<uint8_t>*, Thing::CoAP::IPAddress*, int*));
			};
		}
	}
}