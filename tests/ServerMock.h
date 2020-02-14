#pragma once

#include "gmock/gmock.h"
#include "../Thing.CoAP/IServer.h"
#include "../Thing.CoAP/FunctionalResource.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class ServerMock : public virtual Thing::CoAP::IServer
			{
			public:
				MOCK_METHOD1(SetPort, void(int port));
				MOCK_METHOD0(GetPort, int());

				MOCK_METHOD3(CreateResource, FunctionalResource& (std::string, Thing::CoAP::ContentFormat, bool));
				MOCK_METHOD1(AddResource, void(Thing::CoAP::IResource* endpoint));
				MOCK_METHOD1(AddResource, void(Thing::CoAP::IResource& endpoint));

				MOCK_METHOD1(RemoveResource, void(Thing::CoAP::IResource* endpoint));
				MOCK_METHOD1(RemoveResource, void(Thing::CoAP::IResource& endpoint));

				MOCK_METHOD0(Start, void());
				MOCK_METHOD0(Process, void());
				MOCK_METHOD0(Stop, void());

				MOCK_METHOD2(NotifyObservers, void(Thing::CoAP::IResource*, Thing::CoAP::Status));
				MOCK_METHOD2(NotifyObservers, void(Thing::CoAP::IResource&, Thing::CoAP::Status));
			};
		}
	}
}