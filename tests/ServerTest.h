#pragma once

#include "gtest/gtest.h"
#include "../Thing.CoAP/Server.h"
#include "PacketProviderMock.h"
#include "ResourceMock.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class ServerTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				void BaseEndpointTest(Thing::CoAP::Method method, Thing::CoAP::IResource& endpoint);
				void SimpleEndpointTest(Thing::CoAP::Method method);
				void FunctionalEndpointTest(Thing::CoAP::Method method);
				void EndpointNotFoundTest(Thing::CoAP::Method method);
				void MultipleEndpointTest(ResourceMock* endpointMocks, int size, int callIndex, Thing::CoAP::Method method);
				void ResourceDiscoveryTest(std::vector<uint8_t>* responseBuffer);
				void ObserveTest(bool observe, Thing::CoAP::IPAddress ip, int port, std::vector<uint8_t> tokens, std::string endpoint, uint8_t** response, int* responseLength);

				Thing::CoAP::Server Server;
				PacketProviderMock udpProviderMock;
			};
		}
	}
}