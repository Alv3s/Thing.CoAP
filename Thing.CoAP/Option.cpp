#include "Option.h"
#include <cstddef>

namespace Thing {
	namespace CoAP
	{
		Thing::CoAP::OptionValue Option::GetNumber()
		{
			return number;
		}

		uint16_t Option::GetLenght()
		{
			return static_cast<uint16_t>(buffer.size());
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

		Option* Option::Find(std::vector<Option>& options, Thing::CoAP::OptionValue value)
		{
			for (size_t begin = 0, end = options.size() - 1, p = (end + begin)/2; ; p = (end + begin) / 2)
			{
				Thing::CoAP::Option& option = options[p];
				Thing::CoAP::OptionValue optionValue = option.GetNumber();
				if(value == optionValue)
					return &option;

				if (value < optionValue)
				{
					if (p == 0)
						return NULL;
					end = p - 1;
				}
				else
				{
					if (p == end)
						return NULL;
					begin = p + 1;
				}
			}
			return NULL;
		}

	}
}