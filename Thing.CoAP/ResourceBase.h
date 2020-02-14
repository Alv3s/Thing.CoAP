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

			virtual Thing::CoAP::Status Get(Thing::CoAP::Request& request) override = 0;
			virtual Thing::CoAP::Status Post(Thing::CoAP::Request& request) override = 0;
			virtual Thing::CoAP::Status Put(Thing::CoAP::Request& request) override = 0;
			virtual Thing::CoAP::Status Delete(Thing::CoAP::Request& request) override = 0;

			virtual void ObservableChanged(Thing::CoAP::Status response) override;

		private:
			Thing::CoAP::IServer* server;
			void SetServer(Thing::CoAP::IServer* server) override;
			void SetServer(Thing::CoAP::IServer& server) override;
		};
	}
}

