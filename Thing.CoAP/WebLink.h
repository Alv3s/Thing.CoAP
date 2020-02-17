#pragma once

#include <string>
#include "ContentFormat.h"
#include <list>

namespace Thing
{
	namespace CoAP
	{
		class WebLink
		{
			friend class Client;
		public:
			WebLink(std::string rawData);
			~WebLink();

			std::string GetURI() const;
			std::string GetResourceType() const;
			std::string GetInterfaceDescription() const;
			size_t GetMaximumSizeEstimate() const;
			ContentFormat GetContentFormat() const;
			bool IsObservable() const;
			std::string GetTitle() const;

			bool HasResourceType() const;
			bool HasInterfaceDescription() const;
			bool HasMaximumSizeEstimate() const;
			bool HasTitle() const;
			static std::list<WebLink> ParseString(std::string rawData);
		private:
			std::string uri;

			std::string resourceType;
			bool hasResourceType;

			std::string interfaceDescription;
			bool hasInterfaceDescription;

			size_t maximumSizeEstimate;
			bool hasMaximumSizeEstimate;

			ContentFormat contentFormat;
			bool isObservable;

			std::string title;
			bool hasTitle;
		};
	}
}

