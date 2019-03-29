#include "Option.h"

namespace Thing {
	namespace CoAP
	{
		Thing::CoAP::OptionValue Option::GetNumber()
		{
			return number;
		}

		uint16_t Option::GetLenght()
		{
			return buffer.size();
		}

		uint8_t* Option::GetBuffer()
		{
			return &buffer[0];
		}



		void Option::SetNumber(Thing::CoAP::OptionValue number)
		{
			this->number = number;
		}

		void Option::SetNumber(int number)
		{
			this->number = static_cast<Thing::CoAP::OptionValue>(number);
		}

		void Option::SetOption(uint8_t* buffer, uint16_t length)
		{
			for (int i = 0; i < length; ++i)
				this->buffer.push_back(buffer[i]);
		}
	}
}