#include "WebLink.h"

namespace Thing
{
	namespace CoAP
	{
		WebLink::WebLink(std::string rawData) :
			uri(),
			resourceType(),
			hasResourceType(false),
			interfaceDescription(),
			hasInterfaceDescription(),
			maximumSizeEstimate(),
			hasMaximumSizeEstimate(false),
			contentFormat(),
			isObservable(false),
			title(),
			hasTitle(false)
		{
			//If it doesn't start with "</" then it is invalid
			if (rawData.find("</") != 0)
				return;

			size_t endURI = rawData.find('>');

			this->uri = rawData.substr(2, endURI - 2);

			std::string attributes = rawData.substr(endURI + 2);

			for(size_t start = 0, end = attributes.find(';'); start < attributes.size(); start = end + 1, end = attributes.find(';', start))
			{
				if (end == std::string::npos)
					end = attributes.size();

				size_t equalsIndex = attributes.find('=', start);

				//If the next equals is greater than the string end, then this attribute must be one without equals
				if (equalsIndex > end)
				{
					std::string type = attributes.substr(start, end - start);
					if (type == "obs")
						this->isObservable = true;
					continue;
				}
				

				std::string type = attributes.substr(start, equalsIndex - start);
				std::string value = attributes.substr(equalsIndex + 1, end - equalsIndex - 1);

				if (type == "rt")
				{
					this->resourceType = value.substr(1, value.size() - 2); //Resource Type is a string and we must remove the quotes.
					this->hasResourceType = true;
				}
				else if (type == "if")
				{
					this->interfaceDescription = value.substr(1, value.size() - 2); //Interface Description is a string and we must remove the quotes.
					this->hasInterfaceDescription = true;
				}
				else if (type == "sz")
				{
					sscanf(value.c_str(), "%zu", &this->maximumSizeEstimate);
					this->hasMaximumSizeEstimate = true;
				}
				else if (type == "ct")
				{
				    int contentFormat = 0;
					sscanf(value.c_str(), "%d", &contentFormat);
					this->contentFormat = static_cast<ContentFormat>(contentFormat);
				}
				else if (type == "title")
				{
					this->title = value.substr(1, value.size() - 2); //Title is a string and we must remove the quotes.
					this->hasTitle = true;
				}
			}
		}

		WebLink::~WebLink()
		{
		}

		std::string WebLink::GetURI() const
		{
			return uri;
		}

		std::string WebLink::GetResourceType() const
		{
			return resourceType;
		}

		std::string WebLink::GetInterfaceDescription() const
		{
			return interfaceDescription;
		}

		size_t WebLink::GetMaximumSizeEstimate() const
		{
			return maximumSizeEstimate;
		}

		ContentFormat WebLink::GetContentFormat() const
		{
			return contentFormat;
		}

		bool WebLink::IsObservable() const
		{
			return isObservable;
		}
		std::string WebLink::GetTitle() const
		{
			return title;
		}
		bool WebLink::HasResourceType() const
		{
			return hasResourceType;
		}

		bool WebLink::HasInterfaceDescription() const
		{
			return hasInterfaceDescription;
		}

		bool WebLink::HasMaximumSizeEstimate() const
		{
			return hasMaximumSizeEstimate;
		}

		bool WebLink::HasTitle() const
		{
			return hasTitle;
		}

		std::list<WebLink> WebLink::ParseString(std::string rawData)
		{
			std::list<WebLink> result = std::list<WebLink>();
			for (size_t start = 0, end = rawData.find(','); start < rawData.size(); start = end + 1, end = rawData.find(',', start))
			{
				if (end == std::string::npos)
					end = rawData.size();

				result.push_back(WebLink(rawData.substr(start, end - start)));
			}
			return result;
		}
	}
}