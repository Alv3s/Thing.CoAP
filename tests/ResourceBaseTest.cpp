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
		}
	}
}