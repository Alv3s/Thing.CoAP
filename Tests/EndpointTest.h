#pragma once

#include "gtest/gtest.h"
#include "CoAPServerMock.h"
#include "EndpointMock.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class EndpointTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				CoAPServerMock server;
				EndpointMock endpoint;
			};
		}
	}
}