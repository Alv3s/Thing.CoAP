#include "ResourceBase.h"

namespace Thing {
	namespace CoAP
	{
		ResourceBase::ResourceBase() : server(nullptr)
		{

		}

		void ResourceBase::ObservableChanged(Thing::CoAP::Status response)
		{
			if (server)
				server->NotifyObservers(this, response);
		}

		void ResourceBase::SetServer(Thing::CoAP::IServer* server)
		{
			this->server = server;
		}

		void ResourceBase::SetServer(Thing::CoAP::IServer & server)
		{
			SetServer(&server);
		}
	}
}