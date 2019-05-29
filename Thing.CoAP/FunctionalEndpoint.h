#pragma once

#include "EndpointBase.h"
#include <functional>

namespace Thing 
{
	namespace CoAP
	{
		using EndpointCallback = std::function<Thing::CoAP::Status(Thing::CoAP::Request& request)>;

		class FunctionalEndpoint : public EndpointBase, public virtual IEndpoint
		{
		public:
			FunctionalEndpoint(std::string name, Thing::CoAP::ContentFormat content, bool isObservable);
			~FunctionalEndpoint();

			std::string GetEndpoint() const override;
			Thing::CoAP::ContentFormat GetContentFormat() const override;
			bool IsObservable() const override;

			Thing::CoAP::Status Get(Thing::CoAP::Request& request) override;
			Thing::CoAP::Status Post(Thing::CoAP::Request& request) override;
			Thing::CoAP::Status Put(Thing::CoAP::Request& request) override;
			Thing::CoAP::Status Delete(Thing::CoAP::Request& request) override;

			FunctionalEndpoint& OnGet(EndpointCallback callback);
			FunctionalEndpoint& OnPost(EndpointCallback callback);
			FunctionalEndpoint& OnPut(EndpointCallback callback);
			FunctionalEndpoint& OnDelete(EndpointCallback callback);
		private:
			const std::string name;
			const Thing::CoAP::ContentFormat contentFormat;
			const bool isObservable;

			EndpointCallback getFunction;
			EndpointCallback postFunction;
			EndpointCallback putFunction;
			EndpointCallback deleteFunction;

			static Thing::CoAP::Status defaultAction(Thing::CoAP::Request& request);
		};
	}
}
