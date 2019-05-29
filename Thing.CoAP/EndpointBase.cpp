#include "EndpointBase.h"

namespace Thing {
	namespace CoAP
	{
		EndpointBase::EndpointBase() : server(nullptr)
		{

		}

		void EndpointBase::ObservableChanged(Thing::CoAP::Status response)
		{
			if (server)
				server->NotifyObservers(this, response);
		}

		void EndpointBase::SetServer(Thing::CoAP::IServer* server)
		{
			this->server = server;
		}

		void EndpointBase::SetServer(Thing::CoAP::IServer & server)
		{
			SetServer(&server);
		}
	}
}