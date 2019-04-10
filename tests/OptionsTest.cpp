#include "OptionsTest.h"
#include "../Option.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			void OptionsTest::SetUp()
			{

			}

			void OptionsTest::TearDown()
			{

			}

			TEST_F(OptionsTest, CoAPOptionsObjectFill)
			{
				const uint8_t number = 10;
				const uint8_t buffer[] = { 0, 1, 2, 3,4, 5, 6, 7, 8, 9 };

				Thing::CoAP::Option option;
				option.SetNumber(number);
				option.SetOption((uint8_t*)buffer, sizeof(buffer));

				EXPECT_EQ(number, static_cast<int>(option.GetNumber()));
				EXPECT_EQ(sizeof(buffer), option.GetLenght());
				EXPECT_EQ(0, memcmp(buffer, option.GetBuffer(), sizeof(buffer)));
			}
		}
	}
}