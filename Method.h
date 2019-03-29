#pragma once

namespace Thing {
	namespace CoAP {
		enum class Method
		{
			Empty = 0,
			Get = 1,
			Post = 2,
			Put = 3,
			Delete = 4,
			Content = 69,
			NotFound = 132,
		};
	}
}