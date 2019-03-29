#pragma once

#include "gtest/gtest.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class ObserverTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;
			};
		}
	}
}