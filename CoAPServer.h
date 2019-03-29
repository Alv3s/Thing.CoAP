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

			virtual void AddEndpoint(Thing::CoAP::IEndpoint* endpoint) override;
			virtual void AddEndpoint(Thing::CoAP::IEndpoint& endpoint) override;

			virtual void RemoveEndpoint(Thing::CoAP::IEndpoint* endpoint) override;
			virtual void RemoveEndpoint(Thing::CoAP::IEndpoint& endpoint) override;

			virtual void SetPort(int port) override;
			virtual int GetPort() override;

			virtual void Start() override;
			virtual void Stop() override;
			virtual void Process() override;

			virtual void NotifyObservers(Thing::CoAP::IEndpoint* endpoint, Thing::CoAP::Status r) override;
			virtual void NotifyObservers(Thing::CoAP::IEndpoint& endpoint, Thing::CoAP::Status r) override;

			void SetPacketProvider(IPacketProvider* provider);
			void SetPacketProvider(IPacketProvider& provider);
		private:
			IPacketProvider* udpProvider;
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