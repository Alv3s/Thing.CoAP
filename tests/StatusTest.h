#pragma once

#include "gtest/gtest.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class StatusTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;
			};
		}
	}
}