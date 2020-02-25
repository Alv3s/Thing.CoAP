#include "OptionsTest.h"
#include "../Thing.CoAP/Option.h"

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
				const uint8_t buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

				Thing::CoAP::Option option;
				option.SetNumber(number);
				option.SetOption((uint8_t*)buffer, sizeof(buffer));

				EXPECT_EQ(number, static_cast<uint8_t>(option.GetNumber()));
				EXPECT_EQ(sizeof(buffer), option.GetLenght());
				EXPECT_EQ(0, memcmp(buffer, option.GetBuffer(), sizeof(buffer)));
			}

			TEST_F(OptionsTest, Find_SingleValue_Found)
			{
				const uint8_t number = 10;
				const uint8_t buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

				Thing::CoAP::Option option;
				option.SetNumber(number);
				option.SetOption((uint8_t*)buffer, sizeof(buffer));

				std::vector<Thing::CoAP::Option> options;
				options.push_back(option);

				Option* result = Thing::CoAP::Option::Find(options, static_cast<Thing::CoAP::OptionValue>(number));
				ASSERT_NE(result, nullptr);
				EXPECT_EQ(number, static_cast<uint8_t>(result->GetNumber()));
				EXPECT_EQ(sizeof(buffer), result->GetLenght());
				EXPECT_EQ(0, memcmp(buffer, result->GetBuffer(), sizeof(buffer)));
			}

			TEST_F(OptionsTest, Find_SingleValue_NotFoundHigher)
			{
				const uint8_t number = 10;
				const uint8_t buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

				Thing::CoAP::Option option;
				option.SetNumber(number);
				option.SetOption((uint8_t*)buffer, sizeof(buffer));

				std::vector<Thing::CoAP::Option> options;
				options.push_back(option);

				Option* result = Thing::CoAP::Option::Find(options, static_cast<Thing::CoAP::OptionValue>(number+1));
				ASSERT_EQ(result, nullptr);
			}

			TEST_F(OptionsTest, Find_SingleValue_NotFoundLower)
			{
				const uint8_t number = 10;
				const uint8_t buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

				Thing::CoAP::Option option;
				option.SetNumber(number);
				option.SetOption((uint8_t*)buffer, sizeof(buffer));

				std::vector<Thing::CoAP::Option> options;
				options.push_back(option);

				Option* result = Thing::CoAP::Option::Find(options, static_cast<Thing::CoAP::OptionValue>(number - 1));
				ASSERT_EQ(result, nullptr);
			}

			TEST_F(OptionsTest, Find_MultipleValue_Found)
			{
				const uint8_t number = 10;
				const uint8_t buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				const size_t totalOptions = 10;

				std::vector<Thing::CoAP::Option> options;
				for (size_t i = 0; i < totalOptions; ++i)
				{
					Thing::CoAP::Option option;
					option.SetNumber(number + i);
					option.SetOption((uint8_t*)buffer + i, sizeof(buffer) - i);
					options.push_back(option);
				}

				for (size_t i = 0; i < totalOptions; ++i)
				{
					Option* result = Thing::CoAP::Option::Find(options, static_cast<Thing::CoAP::OptionValue>(number + i));
					ASSERT_NE(result, nullptr);
					EXPECT_EQ(number + i, static_cast<uint8_t>(result->GetNumber()));
					EXPECT_EQ(sizeof(buffer) - i, result->GetLenght());
					EXPECT_EQ(0, memcmp(buffer + i, result->GetBuffer(), sizeof(buffer) - i));
				}
			}

			TEST_F(OptionsTest, Find_MultipleValue_NotFoundLower)
			{
				const uint8_t number = 10;
				const uint8_t buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				const size_t totalOptions = 10;

				std::vector<Thing::CoAP::Option> options;
				for (size_t i = 0; i < totalOptions; ++i)
				{
					Thing::CoAP::Option option;
					option.SetNumber(number + i);
					option.SetOption((uint8_t*)buffer + i, sizeof(buffer) - i);
					options.push_back(option);
				}

				Option* result = Thing::CoAP::Option::Find(options, static_cast<Thing::CoAP::OptionValue>(number - 1));
				ASSERT_EQ(result, nullptr);
			}

			TEST_F(OptionsTest, Find_MultipleValue_NotFoundHigher)
			{
				const uint8_t number = 10;
				const uint8_t buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				const size_t totalOptions = 10;

				std::vector<Thing::CoAP::Option> options;
				for (size_t i = 0; i < totalOptions; ++i)
				{
					Thing::CoAP::Option option;
					option.SetNumber(number + i);
					option.SetOption((uint8_t*)buffer + i, sizeof(buffer) - i);
					options.push_back(option);
				}

				Option* result = Thing::CoAP::Option::Find(options, static_cast<Thing::CoAP::OptionValue>(number + totalOptions));
				ASSERT_EQ(result, nullptr);
			}
		}
	}
}