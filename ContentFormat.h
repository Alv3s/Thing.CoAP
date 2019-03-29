#pragma once

namespace Thing {
	namespace CoAP {
		enum class ContentFormat
		{
			TextPlain = 0,
			ApplicationLinkFormat = 40,
			ApplicationXML = 41,
			ApplicationOctetStream = 42,
			ApplicationEXI = 47,
			ApplicationJSon = 50
		};
	}
}