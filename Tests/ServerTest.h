#pragma once

#include "gtest/gtest.h"
#include "../CoAPServer.h"
#include "PacketProviderMock.h"
#include "CoAPEndpointMock.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class ServerTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				void SimpleEndpointTest(Thing::CoAP::Method method);
				void EndpointNotFoundTest(Thing::CoAP::Method method);
				void MultipleEndpointTest(CoAPEndpointMock* endpointMocks, int size, int callIndex, Thing::CoAP::Method method);
				void ResourceDiscoveryTest(uint8_t** buffer, int* length);
				void ObserveTest(bool observe, Thing::CoAP::IPAddress ip, int port, std::vector<uint8_t> tokens, std::string endpoint, uint8_t** response, int* responseLength);

				Thing::CoAP::Server Server;
				PacketProviderMock udpProviderMock;
			};
		}
	}
}