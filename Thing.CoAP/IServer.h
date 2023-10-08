#pragma once

#include <string>
#include "Status.h"
#include "IResource.h"
#include "IFunctionalResource.h"
#include "IPAddress.h"

namespace Thing
{
	namespace CoAP
	{
		class IServer
		{
		public:
			virtual void SetPort(int port) = 0;
			virtual int GetPort() = 0;

			virtual Thing::CoAP::IFunctionalResource &CreateResource(std::string name, Thing::CoAP::ContentFormat content, bool IsObservable) = 0;
			virtual void AddResource(Thing::CoAP::IResource *endpoint) = 0;
			virtual void AddResource(Thing::CoAP::IResource &endpoint) = 0;

			virtual void RemoveResource(Thing::CoAP::IResource *endpoint) = 0;
			virtual void RemoveResource(Thing::CoAP::IResource &endpoint) = 0;

			virtual void Start() = 0;
			virtual void Process() = 0;
			virtual void Process(void (*onMethodCode)(Thing::CoAP::Method, Thing::CoAP::MessageType)) = 0;
			virtual void Stop() = 0;

			virtual void NotifyObservers(Thing::CoAP::IResource *endpoint, Thing::CoAP::Status r) = 0;
			virtual void NotifyObservers(Thing::CoAP::IResource &endpoint, Thing::CoAP::Status r) = 0;
		};
	}
}