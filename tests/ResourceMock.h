#pragma once

#include "gmock/gmock.h"
#include "../Thing.CoAP/IResource.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			class ResourceMock : public virtual Thing::CoAP::IResource
			{
			public:
				MOCK_CONST_METHOD0(GetName, std::string());
				MOCK_CONST_METHOD0(GetContentFormat, Thing::CoAP::ContentFormat());
				MOCK_CONST_METHOD0(IsObservable, bool());
				MOCK_CONST_METHOD0(GetResourceType, std::string());
				MOCK_CONST_METHOD0(GetTitle, std::string());
				MOCK_CONST_METHOD0(GetMaximumSizeEstimate, size_t());
				MOCK_CONST_METHOD0(GetInterfaceDescription, std::string());

				MOCK_METHOD1(Get, Thing::CoAP::Status(Thing::CoAP::Request&));
				MOCK_METHOD1(Post, Thing::CoAP::Status(Thing::CoAP::Request&));
				MOCK_METHOD1(Put, Thing::CoAP::Status(Thing::CoAP::Request&));
				MOCK_METHOD1(Delete, Thing::CoAP::Status(Thing::CoAP::Request&));

				MOCK_METHOD1(SetServer, void(Thing::CoAP::IServer*));
				MOCK_METHOD1(SetServer, void(Thing::CoAP::IServer&));
				MOCK_METHOD1(ObservableChanged, void(Thing::CoAP::Status));
			};
		}
	}
}