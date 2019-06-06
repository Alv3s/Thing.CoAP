#pragma once

#include <functional>
#include "Status.h"
#include "Request.h"
#include "IEndpoint.h"

namespace Thing
{
	namespace CoAP
	{
		using EndpointCallback = std::function<Thing::CoAP::Status(Thing::CoAP::Request& request)>;

		class IFunctionalEndpoint : private virtual IEndpoint
		{
		public:
			using IEndpoint::ObservableChanged;

			virtual IFunctionalEndpoint& OnGet(EndpointCallback callback) = 0;
			virtual IFunctionalEndpoint& OnPost(EndpointCallback callback) = 0;
			virtual IFunctionalEndpoint& OnPut(EndpointCallback callback) = 0;
			virtual IFunctionalEndpoint& OnDelete(EndpointCallback callback) = 0;
		};
	}
}
