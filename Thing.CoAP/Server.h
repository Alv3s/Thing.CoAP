#pragma once

#include "IServer.h"
#include <map>
#include <string>
#include <list>
#include "Observer.h"
#include "IPacketProvider.h"

namespace Thing {
	namespace CoAP
	{
		class Server : public virtual Thing::CoAP::IServer
		{
		public:
			Server();
			virtual ~Server();

			IFunctionalEndpoint& CreateEndpoint(std::string name, Thing::CoAP::ContentFormat content, bool IsObservable = true) override;

			void AddEndpoint(Thing::CoAP::IEndpoint* endpoint) override;
			void AddEndpoint(Thing::CoAP::IEndpoint& endpoint) override;

			void RemoveEndpoint(Thing::CoAP::IEndpoint* endpoint) override;
			void RemoveEndpoint(Thing::CoAP::IEndpoint& endpoint) override;

			void SetPort(int port) override;
			int GetPort() override;

			void Start() override;
			void Stop() override;
			void Process() override;

			void NotifyObservers(Thing::CoAP::IEndpoint* endpoint, Thing::CoAP::Status r) override;
			void NotifyObservers(Thing::CoAP::IEndpoint& endpoint, Thing::CoAP::Status r) override;

			void SetPacketProvider(IPacketProvider* provider);
			void SetPacketProvider(IPacketProvider& provider);
		private:
			IPacketProvider* packetProvider;
			int port;
			std::map<std::string, Thing::CoAP::IEndpoint*> endpoints;
			std::map<std::string, std::list<Thing::CoAP::Observer> > observers;

			void addObserver(std::string& url, Thing::CoAP::Observer& obs);
			void removeObserver(std::string& url, Thing::CoAP::Observer& obs);

			void resourceDiscovery(Thing::CoAP::Response* response, IPAddress ip, int port);
			void noEndpointDefinedResponse(Thing::CoAP::Response* response, IPAddress ip, int port);
		};
	}
}