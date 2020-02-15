#include "ObserverTest.h"
#include "../Thing.CoAP/Observer.h"
#include "../Thing.CoAP/Functions.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			void ObserverTest::SetUp()
			{

			}

			void ObserverTest::TearDown()
			{

			}

			TEST_F(ObserverTest, FillStructureTest)
			{
				const int port = 12345;
				const Thing::CoAP::IPAddress ip = 98765;
				const uint16_t messageID = Thing::CoAP::Functions::GenerateMessageID();
				std::vector<uint8_t> tokens;
				for (int i = 0; i < 4; ++i)
					tokens.push_back(i);
				Thing::CoAP::Observer observer(ip, port, messageID, tokens);

				EXPECT_EQ(port, observer.GetPort());
				EXPECT_EQ(ip, observer.GetIPAddress());
				EXPECT_EQ(tokens.size(), observer.GetTokens().size());
				EXPECT_TRUE(0 == std::memcmp(&tokens[0], &observer.GetTokens()[0], tokens.size()));
			}

			TEST_F(ObserverTest, CountTest)
			{
				const int totalCounts = 5;

				const int port = 12345;
				const Thing::CoAP::IPAddress ip = 98765;
				const uint16_t messageID = Thing::CoAP::Functions::GenerateMessageID();
				std::vector<uint8_t> tokens;
				for (int i = 0; i < 4; ++i)
					tokens.push_back(i);
				Thing::CoAP::Observer observer(ip, port, messageID, tokens);


				for(uint16_t i = 1; i <= totalCounts; ++i)
					EXPECT_EQ(i, observer.NextCount());
			}

			TEST_F(ObserverTest, MessageIDTest)
			{
				const int totalCounts = 5;

				const int port = 12345;
				const Thing::CoAP::IPAddress ip = 98765;
				const uint16_t messageID = Thing::CoAP::Functions::GenerateMessageID();
				std::vector<uint8_t> tokens;
				for (int i = 0; i < 4; ++i)
					tokens.push_back(i);
				Thing::CoAP::Observer observer(ip, port, messageID, tokens);


				for (uint16_t i = 0; i < totalCounts; ++i)
					EXPECT_EQ(i+messageID, observer.NextMessageID());
			}
		}
	}
}