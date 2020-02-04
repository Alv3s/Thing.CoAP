#pragma once

#include "gmock/gmock.h"
#include "..//Thing.CoAP/Option.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			ACTION_TEMPLATE(CopyBuffer, HAS_2_TEMPLATE_PARAMS(int, a, int, b), AND_2_VALUE_PARAMS(pointer, length))
			{
				const void* bufferData = (void*)::std::tr1::get<a>(args);
				const void* lengthData = (void*)::std::tr1::get<b>(args);

				uint8_t* buffer = new uint8_t[(int)lengthData];
				std::memcpy(buffer, bufferData, (int)lengthData);

				*pointer = buffer;
				*length = (int)lengthData;
			}

			MATCHER_P(ContentFormat, contentformat, "")
			{
				Thing::CoAP::Option value = (Thing::CoAP::Option)arg;

				if (value.GetNumber() != Thing::CoAP::OptionValue::ContentFormat)
					return false;

				uint32_t aux = 0;
				for (int i = 0; i < value.GetLenght(); ++i)
				{
					aux = aux << 8;
					aux |= value.GetBuffer()[i];
				}
				return aux == static_cast<uint32_t>(contentformat);
			}

			MATCHER_P3(Option, optionNumber, payload, payloadSize, "")
			{
				Thing::CoAP::Option value = (Thing::CoAP::Option)arg;

				if (value.GetNumber() != optionNumber)
					return false;

				if (!payload && payloadSize == value.GetLenght() && value.GetLenght() == 0)
					return true;

				if (!payload)
					return false;

				if (0 == std::memcmp(payload, value.GetBuffer(), value.GetLenght()))
					return true;

				return false;
			}
		}
	}
}