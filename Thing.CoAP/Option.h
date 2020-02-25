#pragma once

#include <cstdint>
#include "OptionValue.h"
#include <vector>

namespace Thing {
	namespace CoAP
	{
		class Option
		{
		public:
			Thing::CoAP::OptionValue GetNumber();
			uint16_t GetLenght();
			uint8_t* GetBuffer();

			void SetNumber(Thing::CoAP::OptionValue number);
			void SetNumber(int number);
			void SetOption(uint8_t* buffer, uint16_t length);

			static Option* Find(std::vector<Option>& options, Thing::CoAP::OptionValue value);
		private:
			Thing::CoAP::OptionValue number;
			std::vector<uint8_t> buffer;
		};
	}
}