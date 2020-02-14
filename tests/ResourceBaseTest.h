#pragma once

#include "gtest/gtest.h"
#include "ServerMock.h"
#include "ResourceBaseMock.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class ResourceBaseTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				ServerMock server;
				ResourceBaseMock endpoint;
			};
		}
	}
}