#include "CoAPEndpoint.h"

namespace Thing {
	namespace CoAP
	{
		Endpoint::Endpoint() : server(nullptr)
		{

		}

		void Endpoint::ObservableChanged(Thing::CoAP::Status response)
		{
			if (server)
				server->NotifyObservers(this, response);
		}

		void Endpoint::SetServer(Thing::CoAP::IServer* server)
		{
			this->server = server;
		}

		void Endpoint::SetServer(Thing::CoAP::IServer & server)
		{
			SetServer(&server);
		}
	}
}