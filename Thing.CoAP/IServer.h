#pragma once

#include <string>
#include "Status.h"
#include "IEndpoint.h"
#include "IFunctionalEndpoint.h"

namespace Thing {
	namespace CoAP
	{
		typedef uint32_t IPAddress;

		class IServer
		{
		public:
			virtual void SetPort(int port) = 0;
			virtual int GetPort() = 0;

			virtual Thing::CoAP::IFunctionalEndpoint& CreateEndpoint(std::string name, Thing::CoAP::ContentFormat content, bool IsObservable) = 0;
			virtual void AddEndpoint(Thing::CoAP::IEndpoint* endpoint) = 0;
			virtual void AddEndpoint(Thing::CoAP::IEndpoint& endpoint) = 0;

			virtual void RemoveEndpoint(Thing::CoAP::IEndpoint* endpoint) = 0;
			virtual void RemoveEndpoint(Thing::CoAP::IEndpoint& endpoint) = 0;

			virtual void Start() = 0;
			virtual void Process() = 0;
			virtual void Stop() = 0;

			virtual void NotifyObservers(Thing::CoAP::IEndpoint* endpoint, Thing::CoAP::Status r) = 0;
			virtual void NotifyObservers(Thing::CoAP::IEndpoint& endpoint, Thing::CoAP::Status r) = 0;
		};
	}
}