#pragma once

#include "EndpointBase.h"
#include "IFunctionalEndpoint.h"

namespace Thing 
{
	namespace CoAP
	{

		class FunctionalEndpoint : public EndpointBase, public virtual IEndpoint, public virtual IFunctionalEndpoint
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

			FunctionalEndpoint& OnGet(EndpointCallback callback) override;
			FunctionalEndpoint& OnPost(EndpointCallback callback) override;
			FunctionalEndpoint& OnPut(EndpointCallback callback) override;
			FunctionalEndpoint& OnDelete(EndpointCallback callback) override;
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
