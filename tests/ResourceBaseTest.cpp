#include "ResourceBaseTest.h"

using ::testing::_;

namespace Thing {
	namespace CoAP {
		namespace Tests {
			void ResourceBaseTest::SetUp()
			{

			}

			void ResourceBaseTest::TearDown()
			{

			}

			TEST_F(ResourceBaseTest, ObservableChangedViaReference)
			{
				Thing::CoAP::IResource& iEndpoint = endpoint;
				iEndpoint.SetServer(server);

				Thing::CoAP::Status status = Thing::CoAP::Status::Valid();
				EXPECT_CALL(server, NotifyObservers(&endpoint, status)).Times(1);
				endpoint.ObservableChanged(status);
			}

			TEST_F(ResourceBaseTest, ObservableChangedViaPointer)
			{
				Thing::CoAP::IResource& iEndpoint = endpoint;
				iEndpoint.SetServer(&server);

				Thing::CoAP::Status status = Thing::CoAP::Status::Valid();
				EXPECT_CALL(server, NotifyObservers(&endpoint, status)).Times(1);
				endpoint.ObservableChanged(status);
			}

			TEST_F(ResourceBaseTest, ObservableChangedServerNull)
			{
				Thing::CoAP::IResource& iEndpoint = endpoint;
				iEndpoint.SetServer(nullptr);

				Thing::CoAP::Status status = Thing::CoAP::Status::Valid();
				EXPECT_CALL(server, NotifyObservers(&endpoint, _)).Times(0);
				endpoint.ObservableChanged(status);
			}

			TEST_F(ResourceBaseTest, ResourceTypeTest)
			{
				const std::string resourceType = "Some Resource";
				endpoint.SetResourceType(resourceType);

				EXPECT_EQ(resourceType, endpoint.GetResourceType());
			}

			TEST_F(ResourceBaseTest, InterfaceDescriptionTest)
			{
				const std::string interfaceDescription = "Some Description";
				endpoint.SetInterfaceDescription(interfaceDescription);

				EXPECT_EQ(interfaceDescription, endpoint.GetInterfaceDescription());
			}

			TEST_F(ResourceBaseTest, TitleTest)
			{
				const std::string title = "Some title";
				endpoint.SetTitle(title);

				EXPECT_EQ(title, endpoint.GetTitle());
			}

			TEST_F(ResourceBaseTest, MaximumSizeEstimateTest)
			{
				const size_t maximumSizeEstimate = 10;
				endpoint.SetMaximumSizeEstimate(maximumSizeEstimate);

				EXPECT_EQ(maximumSizeEstimate, endpoint.GetMaximumSizeEstimate());
			}
		}
	}
}