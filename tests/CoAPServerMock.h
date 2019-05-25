#pragma once

#include "gmock/gmock.h"
#include "../IServer.h"
#include "../FunctionalEndpoint.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class CoAPServerMock : public virtual Thing::CoAP::IServer
			{
			public:
				MOCK_METHOD1(SetPort, void(int port));
				MOCK_METHOD0(GetPort, int());

				MOCK_METHOD3(CreateEndpoint, FunctionalEndpoint& (std::string, Thing::CoAP::ContentFormat, bool));
				MOCK_METHOD1(AddEndpoint, void(Thing::CoAP::IEndpoint* endpoint));
				MOCK_METHOD1(AddEndpoint, void(Thing::CoAP::IEndpoint& endpoint));

				MOCK_METHOD1(RemoveEndpoint, void(Thing::CoAP::IEndpoint* endpoint));
				MOCK_METHOD1(RemoveEndpoint, void(Thing::CoAP::IEndpoint& endpoint));

				MOCK_METHOD0(Start, void());
				MOCK_METHOD0(Process, void());
				MOCK_METHOD0(Stop, void());

				MOCK_METHOD2(NotifyObservers, void(Thing::CoAP::IEndpoint*, Thing::CoAP::Status));
				MOCK_METHOD2(NotifyObservers, void(Thing::CoAP::IEndpoint&, Thing::CoAP::Status));
			};
		}
	}
}