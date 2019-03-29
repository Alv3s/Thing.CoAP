#pragma once

namespace Thing {
	namespace CoAP {
		enum class OptionValue
		{
			IfMatch = 1,
			UriHost = 3,
			ETag = 4,
			IfNoneMatch = 5,
			URIPort = 7,
			LocationPath = 8,
			URIPath = 11,
			ContentFormat = 12,
			MaxAge = 14,
			URIQuery = 15,
			Accept = 17,
			LocationQuery = 20,
			Block2 = 23,
			ProxyURI = 35,
			ProxyScheme = 39,
			Observe = 6
		};
	}
}