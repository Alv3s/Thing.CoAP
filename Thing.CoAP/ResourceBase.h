#pragma once

#include "IResource.h"
#include "IServer.h"

namespace Thing {
	namespace CoAP
	{
		class ResourceBase : public virtual IResource
		{
		public:
			ResourceBase();

			virtual std::string GetName() const override = 0;
			virtual Thing::CoAP::ContentFormat GetContentFormat() const override = 0;
			virtual bool IsObservable() const = 0;

			std::string GetResourceType() const override;
			std::string GetInterfaceDescription() const override;
			size_t GetMaximumSizeEstimate() const override;
			std::string GetTitle() const override;

			void SetResourceType(std::string);
			void SetInterfaceDescription(std::string interfaceDescription);
			void SetMaximumSizeEstimate(size_t sz);
			void SetTitle(std::string title);

			virtual Thing::CoAP::Status Get(Thing::CoAP::Request& request) override = 0;
			virtual Thing::CoAP::Status Post(Thing::CoAP::Request& request) override = 0;
			virtual Thing::CoAP::Status Put(Thing::CoAP::Request& request) override = 0;
			virtual Thing::CoAP::Status Delete(Thing::CoAP::Request& request) override = 0;

			virtual void ObservableChanged(Thing::CoAP::Status response) override;
		private:
			Thing::CoAP::IServer* server;
			std::string resourceType;
			std::string interfaceDescription;
			size_t maximumSizeEstimate;
			std::string title;
			void SetServer(Thing::CoAP::IServer* server) override;
			void SetServer(Thing::CoAP::IServer& server) override;
		};
	}
}

