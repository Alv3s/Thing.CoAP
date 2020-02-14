#include "FunctionalResource.h"

namespace Thing {
	namespace CoAP
	{
		FunctionalResource::FunctionalResource(std::string name, Thing::CoAP::ContentFormat content, bool isObservable) :
			name(name),
			contentFormat(content),
			isObservable(isObservable),
			getFunction(FunctionalResource::defaultAction),
			postFunction(FunctionalResource::defaultAction),
			putFunction(FunctionalResource::defaultAction),
			deleteFunction(FunctionalResource::defaultAction)
		{
		}

		FunctionalResource::~FunctionalResource()
		{

		}

		std::string FunctionalResource::GetName() const
		{
			return name;
		}

		Thing::CoAP::ContentFormat FunctionalResource::GetContentFormat() const
		{
			return contentFormat;
		}

		bool FunctionalResource::IsObservable() const
		{
			return isObservable;
		}

		Thing::CoAP::Status FunctionalResource::Get(Thing::CoAP::Request& request)
		{
			return getFunction(request);
		}

		Thing::CoAP::Status FunctionalResource::Post(Thing::CoAP::Request& request)
		{
			return postFunction(request);
		}

		Thing::CoAP::Status FunctionalResource::Put(Thing::CoAP::Request& request)
		{
			return putFunction(request);
		}

		Thing::CoAP::Status FunctionalResource::Delete(Thing::CoAP::Request& request)
		{
			return deleteFunction(request);
		}

		Thing::CoAP::Status FunctionalResource::defaultAction(Thing::CoAP::Request& request)
		{
			return Thing::CoAP::Status::NotImplemented();
		}

		FunctionalResource& FunctionalResource::OnGet(EndpointCallback callback)
		{
			getFunction = callback;
			return *this;
		}

		FunctionalResource& FunctionalResource::OnPost(EndpointCallback callback)
		{
			postFunction = callback;
			return *this;
		}

		FunctionalResource& FunctionalResource::OnPut(EndpointCallback callback)
		{
			putFunction = callback;
			return *this;
		}

		FunctionalResource& FunctionalResource::OnDelete(EndpointCallback callback)
		{
			deleteFunction = callback;
			return *this;
		}
	}
}
