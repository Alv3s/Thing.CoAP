#pragma once

#include "ResourceBase.h"
#include "IFunctionalResource.h"

namespace Thing 
{
	namespace CoAP
	{

		class FunctionalResource : public ResourceBase, public virtual IResource, public virtual IFunctionalResource
		{
		public:
			FunctionalResource(std::string name, Thing::CoAP::ContentFormat content, bool isObservable);
			~FunctionalResource();

			std::string GetName() const override;
			Thing::CoAP::ContentFormat GetContentFormat() const override;
			bool IsObservable() const override;

			Thing::CoAP::Status Get(Thing::CoAP::Request& request) override;
			Thing::CoAP::Status Post(Thing::CoAP::Request& request) override;
			Thing::CoAP::Status Put(Thing::CoAP::Request& request) override;
			Thing::CoAP::Status Delete(Thing::CoAP::Request& request) override;

			FunctionalResource& OnGet(EndpointCallback callback) override;
			FunctionalResource& OnPost(EndpointCallback callback) override;
			FunctionalResource& OnPut(EndpointCallback callback) override;
			FunctionalResource& OnDelete(EndpointCallback callback) override;
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
