#pragma once

#include "gmock/gmock.h"
#include "../Thing.CoAP/EndpointBase.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class EndpointBaseMock : public Thing::CoAP::EndpointBase
			{
			public:
				MOCK_CONST_METHOD0(GetEndpoint, std::string());
				MOCK_CONST_METHOD0(GetContentFormat, Thing::CoAP::ContentFormat());
				MOCK_CONST_METHOD0(IsObservable, bool());

				MOCK_METHOD1(Get, Thing::CoAP::Status(Thing::CoAP::Request&));
				MOCK_METHOD1(Post, Thing::CoAP::Status(Thing::CoAP::Request&));
				MOCK_METHOD1(Put, Thing::CoAP::Status(Thing::CoAP::Request&));
				MOCK_METHOD1(Delete, Thing::CoAP::Status(Thing::CoAP::Request&));
			};
		}
	}
}