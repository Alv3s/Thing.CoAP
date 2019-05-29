#include "EndpointBaseTest.h"

using ::testing::_;

namespace Thing {
	namespace CoAP {
		namespace Tests {
			void EndpointBaseTest::SetUp()
			{

			}

			void EndpointBaseTest::TearDown()
			{

			}

			TEST_F(EndpointBaseTest, ObservableChangedViaReference)
			{
				Thing::CoAP::IEndpoint& iEndpoint = endpoint;
				iEndpoint.SetServer(server);

				Thing::CoAP::Status status = Thing::CoAP::Status::Valid();
				EXPECT_CALL(server, NotifyObservers(&endpoint, status)).Times(1);
				endpoint.ObservableChanged(status);
			}

			TEST_F(EndpointBaseTest, ObservableChangedViaPointer)
			{
				Thing::CoAP::IEndpoint& iEndpoint = endpoint;
				iEndpoint.SetServer(&server);

				Thing::CoAP::Status status = Thing::CoAP::Status::Valid();
				EXPECT_CALL(server, NotifyObservers(&endpoint, status)).Times(1);
				endpoint.ObservableChanged(status);
			}

			TEST_F(EndpointBaseTest, ObservableChangedServerNull)
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