#include "ResourceBase.h"

namespace Thing {
	namespace CoAP
	{
		ResourceBase::ResourceBase() : 
			server(nullptr),
			resourceType(),
			interfaceDescription(),
			maximumSizeEstimate(),
			title()
		{

		}

		std::string ResourceBase::GetResourceType() const
		{
			return resourceType;
		}

		std::string ResourceBase::GetInterfaceDescription() const
		{
			return interfaceDescription;
		}

		size_t ResourceBase::GetMaximumSizeEstimate() const
		{
			return maximumSizeEstimate;
		}

		std::string ResourceBase::GetTitle() const
		{
			return title;
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

		void ResourceBase::SetResourceType(std::string resourceType)
		{
			this->resourceType = resourceType;
		}

		void ResourceBase::SetInterfaceDescription(std::string interfaceDescription)
		{
			this->interfaceDescription = interfaceDescription;
		}

		void ResourceBase::SetMaximumSizeEstimate(size_t sz)
		{
			this->maximumSizeEstimate = sz;
		}

		void ResourceBase::SetTitle(std::string title)
		{
			this->title = title;
		}
	}
}