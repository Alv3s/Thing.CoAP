#pragma once

#include "IServer.h"
#include <map>
#include <string>
#include <list>
#include "Observer.h"
#include "IPacketProvider.h"

namespace Thing
{
	namespace CoAP
	{
		class Server : public virtual Thing::CoAP::IServer
		{
		public:
			Server();
			virtual ~Server();

			IFunctionalResource &CreateResource(std::string name, Thing::CoAP::ContentFormat content, bool IsObservable = true) override;

			void AddResource(Thing::CoAP::IResource *endpoint) override;
			void AddResource(Thing::CoAP::IResource &endpoint) override;

			void RemoveResource(Thing::CoAP::IResource *endpoint) override;
			void RemoveResource(Thing::CoAP::IResource &endpoint) override;

			void SetPort(int port) override;
			int GetPort() override;

			void Start() override;
			void Stop() override;
			void Process() override;
			void Process(void (*onMethodCode)(Thing::CoAP::Method, Thing::CoAP::MessageType)) override;

			void NotifyObservers(Thing::CoAP::IResource *endpoint, Thing::CoAP::Status r) override;
			void NotifyObservers(Thing::CoAP::IResource &endpoint, Thing::CoAP::Status r) override;

			void SetPacketProvider(IPacketProvider *provider);
			void SetPacketProvider(IPacketProvider &provider);

		private:
			struct AvailableResource
			{
				Thing::CoAP::IResource *resource;
				std::list<Thing::CoAP::Observer> observers;
			};

			IPacketProvider *packetProvider;
			int port;
			std::map<std::string, AvailableResource> resources;

			void removeObserver(std::string &url, Thing::CoAP::Observer &obs);

			void resourceDiscovery(Thing::CoAP::Response *response, IPAddress ip, int port);
			void noEndpointDefinedResponse(Thing::CoAP::Response *response, IPAddress ip, int port);
		};
	}
}