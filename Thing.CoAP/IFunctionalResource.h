#pragma once

#include <functional>
#include "Status.h"
#include "Request.h"
#include "IResource.h"

namespace Thing
{
	namespace CoAP
	{
		using EndpointCallback = std::function<Thing::CoAP::Status(Thing::CoAP::Request& request)>;

		class IFunctionalResource : private virtual IResource
		{
		public:
			using IResource::ObservableChanged;

			virtual IFunctionalResource& OnGet(EndpointCallback callback) = 0;
			virtual IFunctionalResource& OnPost(EndpointCallback callback) = 0;
			virtual IFunctionalResource& OnPut(EndpointCallback callback) = 0;
			virtual IFunctionalResource& OnDelete(EndpointCallback callback) = 0;
		};
	}
}
