#pragma once

#include <string>
#include "Request.h"
#include "Response.h"
#include "ContentFormat.h"
#include "Status.h"

namespace Thing {
	namespace CoAP
	{
		class IServer;

		class IEndpoint
		{
		public:
			virtual std::string GetEndpoint() const = 0;
			virtual Thing::CoAP::ContentFormat GetContentFormat() const = 0;
			virtual bool IsObservable() const = 0;

			virtual Thing::CoAP::Status Get(Thing::CoAP::Request& request) = 0;
			virtual Thing::CoAP::Status Post(Thing::CoAP::Request& request) = 0;
			virtual Thing::CoAP::Status Put(Thing::CoAP::Request& request) = 0;
			virtual Thing::CoAP::Status Delete(Thing::CoAP::Request& request) = 0;

			virtual void SetServer(Thing::CoAP::IServer* server) = 0;
			virtual void SetServer(Thing::CoAP::IServer& server) = 0;
			virtual void ObservableChanged(Thing::CoAP::Status response) = 0;
		};
	}
}