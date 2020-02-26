#pragma once

#include "gmock/gmock.h"
#include "../Thing.CoAP/Option.h"
#include <cstring>

namespace Thing {
	namespace CoAP {
		namespace Tests {
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