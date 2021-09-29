#include "WebLinkTest.h"


#include "../Thing.CoAP/WebLink.h"
#include "Common.h"

using ::testing::_;
using ::testing::SaveArg;
using ::testing::Contains;
using ::testing::SetArgPointee;
using ::testing::Return;

namespace Thing {
	namespace CoAP {
		namespace Tests {

			const std::string rawData = "</test>;rt=\"observer\";if=\"thing.coap\";sz=10;ct=50;obs;title=\"Some Title\"";

			void WebLinkTest::SetUp()
			{
			}

			void WebLinkTest::TearDown()
			{
			}

			TEST_F(WebLinkTest, URIParseTest)
			{
				WebLink link(rawData);
				ASSERT_EQ("test", link.GetURI());
			}

			TEST_F(WebLinkTest, ResourceTypeTest)
			{
				WebLink link(rawData);
				ASSERT_TRUE(link.HasResourceType());
				ASSERT_EQ("observer", link.GetResourceType());
			}

			TEST_F(WebLinkTest, ResourceTypeNotExistTest)
			{
				const std::string rawData = "</test>;if=\"thing.coap\";sz=10;ct=50;obs;title=\"Some Title\"";
				WebLink link(rawData);
				ASSERT_FALSE(link.HasResourceType());
			}

			TEST_F(WebLinkTest, InterfaceDescriptionTest)
			{
				WebLink link(rawData);
				ASSERT_TRUE(link.HasInterfaceDescription());
				ASSERT_EQ("thing.coap", link.GetInterfaceDescription());
			}

			TEST_F(WebLinkTest, InterfaceDescriptionNotExistTest)
			{
				const std::string rawData = "</test>;sz=10;ct=50;obs;title=\"Some Title\"";
				WebLink link(rawData);
				ASSERT_FALSE(link.HasInterfaceDescription());
			}

			TEST_F(WebLinkTest, MaximumSizeEstimateTest)
			{
				WebLink link(rawData);
				ASSERT_TRUE(link.HasMaximumSizeEstimate());
				ASSERT_EQ(10ul, link.GetMaximumSizeEstimate());
			}

			TEST_F(WebLinkTest, MaximumSizeEstimateNotExistTest)
			{
				const std::string rawData = "</test>;if=\"thing.coap\";ct=50;obs;title=\"Some Title\"";
				WebLink link(rawData);
				ASSERT_FALSE(link.HasMaximumSizeEstimate());
			}

			TEST_F(WebLinkTest, ContentTypeTest)
			{
				WebLink link(rawData);
				ASSERT_EQ(Thing::CoAP::ContentFormat::ApplicationJSon, link.GetContentFormat());
			}

			TEST_F(WebLinkTest, IsObservableTest)
			{
				WebLink link(rawData);
				ASSERT_EQ(true, link.IsObservable());
			}

			TEST_F(WebLinkTest, IsNotObservableTest)
			{
				const std::string rawData = "</test>;rt=\"observer\";if=\"thing.coap\";sz=10;ct=50;title=\"Some Title\"";

				WebLink link(rawData);
				ASSERT_EQ(false, link.IsObservable());
			}

			TEST_F(WebLinkTest, TitleTest)
			{
				WebLink link(rawData);
				ASSERT_TRUE(link.HasTitle());
				ASSERT_EQ("Some Title", link.GetTitle());
			}

			TEST_F(WebLinkTest, TitleNotExistTest)
			{
				const std::string rawData = "</test>;if=\"thing.coap\";sz=10;ct=50;obs";
				WebLink link(rawData);
				ASSERT_FALSE(link.HasTitle());
			}

			TEST_F(WebLinkTest, ParseString_Single)
			{
				std::list<WebLink> links = WebLink::ParseString(rawData);

				ASSERT_EQ(1ul, links.size());

				WebLink link = *links.begin();
				ASSERT_EQ("test", link.GetURI());
				ASSERT_EQ("observer", link.GetResourceType());
				ASSERT_EQ("thing.coap", link.GetInterfaceDescription());
				ASSERT_EQ(10ul, link.GetMaximumSizeEstimate());
				ASSERT_EQ(Thing::CoAP::ContentFormat::ApplicationJSon, link.GetContentFormat());
				ASSERT_EQ(true, link.IsObservable());
				ASSERT_EQ("Some Title", link.GetTitle());
			}

			TEST_F(WebLinkTest, ParseString_Multiple)
			{
				const std::string rawData = "</test%d>;rt=\"observer%d\";if=\"thing.coap%d\";sz=%d;ct=50;obs;title=\"Some Title%d\"";
				const size_t startSize = 10;
				const size_t totalLinks = 6;
				std::string allData = "";
				for (size_t i = 0; i < totalLinks; ++i)
				{
					if (i != 0)
						allData += ",";
					char buffer[255];
					sprintf(buffer, rawData.c_str(), i, i, i, startSize + i, i);
					allData += std::string(buffer);
				}

				std::list<WebLink> links = WebLink::ParseString(allData);

				ASSERT_EQ(totalLinks, links.size());

				int i = 0;
				for (auto& link : links)
				{
					ASSERT_EQ("test"+std::to_string(i), link.GetURI());
					ASSERT_EQ("observer" + std::to_string(i), link.GetResourceType());
					ASSERT_EQ("thing.coap" + std::to_string(i), link.GetInterfaceDescription());
					ASSERT_EQ(startSize+i, link.GetMaximumSizeEstimate());
					ASSERT_EQ(Thing::CoAP::ContentFormat::ApplicationJSon, link.GetContentFormat());
					ASSERT_EQ(true, link.IsObservable());
					ASSERT_EQ("Some Title" + std::to_string(i), link.GetTitle());

					++i;
				}
			}
		}
	}
}
