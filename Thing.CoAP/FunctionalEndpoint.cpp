#include "FunctionalEndpoint.h"

namespace Thing {
	namespace CoAP
	{
		FunctionalEndpoint::FunctionalEndpoint(std::string name, Thing::CoAP::ContentFormat content, bool isObservable) :
			name(name),
			contentFormat(content),
			isObservable(isObservable),
			getFunction(FunctionalEndpoint::defaultAction),
			postFunction(FunctionalEndpoint::defaultAction),
			putFunction(FunctionalEndpoint::defaultAction),
			deleteFunction(FunctionalEndpoint::defaultAction)
		{
		}

		FunctionalEndpoint::~FunctionalEndpoint()
		{

		}

		std::string FunctionalEndpoint::GetEndpoint() const
		{
			return name;
		}

		Thing::CoAP::ContentFormat FunctionalEndpoint::GetContentFormat() const
		{
			return contentFormat;
		}

		bool FunctionalEndpoint::IsObservable() const
		{
			return isObservable;
		}

		Thing::CoAP::Status FunctionalEndpoint::Get(Thing::CoAP::Request& request)
		{
			return getFunction(request);
		}

		Thing::CoAP::Status FunctionalEndpoint::Post(Thing::CoAP::Request& request)
		{
			return postFunction(request);
		}

		Thing::CoAP::Status FunctionalEndpoint::Put(Thing::CoAP::Request& request)
		{
			return putFunction(request);
		}

		Thing::CoAP::Status FunctionalEndpoint::Delete(Thing::CoAP::Request& request)
		{
			return deleteFunction(request);
		}

		Thing::CoAP::Status FunctionalEndpoint::defaultAction(Thing::CoAP::Request& request)
		{
			return Thing::CoAP::Status::NotImplemented();
		}

		FunctionalEndpoint& FunctionalEndpoint::OnGet(EndpointCallback callback)
		{
			getFunction = callback;
			return *this;
		}

		FunctionalEndpoint& FunctionalEndpoint::OnPost(EndpointCallback callback)
		{
			postFunction = callback;
			return *this;
		}

		FunctionalEndpoint& FunctionalEndpoint::OnPut(EndpointCallback callback)
		{
			putFunction = callback;
			return *this;
		}

		FunctionalEndpoint& FunctionalEndpoint::OnDelete(EndpointCallback callback)
		{
			deleteFunction = callback;
			return *this;
		}
	}
}
