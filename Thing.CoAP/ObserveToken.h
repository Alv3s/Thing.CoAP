#pragma once

#include <string>
#include <cstdint>
#include "ResponseCallback.h"

namespace Thing
{
	namespace CoAP
	{
		class ObserveToken
		{
		public:
			ObserveToken();
			ObserveToken(uint16_t token, std::string endpoint, ResponseCallback callback);
			~ObserveToken();

			uint16_t GetToken();
			std::string GetEndpoint();
			ResponseCallback GetCallback();
		private:
			uint16_t token;
			std::string endpoint;
			ResponseCallback callback;
		};
	}
}
