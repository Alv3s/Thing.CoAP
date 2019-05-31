#pragma once

#include <functional>
#include "Response.h"

namespace Thing
{
	namespace CoAP
	{
		using ResponseCallback = std::function<void(Thing::CoAP::Response& response)>;
	}
}