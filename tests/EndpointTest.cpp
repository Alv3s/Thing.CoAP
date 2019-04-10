#include "EndpointTest.h"

using ::testing::_;

namespace Thing {
	namespace CoAP {
		namespace Tests {
			void EndpointTest::SetUp()
			{

			}

			void EndpointTest::TearDown()
			{

			}

			TEST_F(EndpointTest, ObservableChangedViaReference)
			{
				Thing::CoAP::IEndpoint& iEndpoint = endpoint;
				iEndpoint.SetServer(server);

				Thing::CoAP::Status status = Thing::CoAP::Status::Valid();
				EXPECT_CALL(server, NotifyObservers(&endpoint, status)).Times(1);
				endpoint.ObservableChanged(status);
			}

			TEST_F(EndpointTest, ObservableChangedViaPointer)
			{
				Thing::CoAP::IEndpoint& iEndpoint = endpoint;
				iEndpoint.SetServer(&server);

				Thing::CoAP::Status status = Thing::CoAP::Status::Valid();
				EXPECT_CALL(server, NotifyObservers(&endpoint, status)).Times(1);
				endpoint.ObservableChanged(status);
			}

			TEST_F(EndpointTest, ObservableChangedServerNull)
			{
				Thing::CoAP::IEndpoint& iEndpoint = endpoint;
				iEndpoint.SetServer(nullptr);

				Thing::CoAP::Status status = Thing::CoAP::Status::Valid();
				EXPECT_CALL(server, NotifyObservers(&endpoint, _)).Times(0);
				endpoint.ObservableChanged(status);
			}
		}
	}
}