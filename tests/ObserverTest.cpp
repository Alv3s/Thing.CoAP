#include "ObserverTest.h"
#include "../Thing.CoAP/Observer.h"

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
				std::vector<uint8_t> tokens;
				for (int i = 0; i < 4; ++i)
					tokens.push_back(i);
				Thing::CoAP::Observer observer(ip, port, tokens);

				EXPECT_EQ(port, observer.GetPort());
				EXPECT_EQ(ip, observer.GetIPAddress());
				EXPECT_EQ(tokens.size(), observer.GetTokens().size());
				EXPECT_TRUE(0 == std::memcmp(&tokens[0], &observer.GetTokens()[0], tokens.size()));
			}
		}
	}
}