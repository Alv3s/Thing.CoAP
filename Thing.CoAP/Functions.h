#pragma once

#include <stdint.h>
#include <stdlib.h>

namespace Thing {
	namespace CoAP
	{
		class Functions
		{
		public:
			static uint16_t GenerateMessageID()
			{
				return rand() % 65536 + 1;
			}
		};
	}
}