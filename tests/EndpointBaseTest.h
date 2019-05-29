#pragma once

#include "gtest/gtest.h"
#include "ServerMock.h"
#include "EndpointBaseMock.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class EndpointBaseTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				ServerMock server;
				EndpointBaseMock endpoint;
			};
		}
	}
}