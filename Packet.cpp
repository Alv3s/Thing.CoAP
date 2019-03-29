#include "Packet.h"
#include <string.h> 

namespace Thing {
	namespace CoAP
	{
		static inline bool GetOption(Thing::CoAP::Option& option, uint32_t *running_delta, uint8_t **buf, size_t buflen)
		{
			uint8_t *p = *buf;
			uint8_t headlen = 1;
			uint16_t len;
			uint32_t delta;

			if (buflen < headlen)
				return true;

			delta = (p[0] & 0xF0) >> 4;
			len = p[0] & 0x0F;


			if (delta == 13)
			{
				headlen++;
				if (buflen < headlen)
					return false;
				delta = p[1] + 13;
				p++;
			}
			else if (delta == 14)
			{
				headlen += 2;
				if (buflen < headlen)
					return false;
				delta = ((p[1] << 8) | p[2]) + 269;
				p += 2;
			}
			else if (delta == 15)
				return false;

			if (len == 13)
			{
				headlen++;
				if (buflen < headlen)
					return false;
				len = p[1] + 13;
				p++;
			}
			else if (len == 14)
			{
				headlen += 2;
				if (buflen < headlen)
					return false;
				len = ((p[1] << 8) | p[2]) + 269;
				p += 2;
			}
			else if (len == 15)
				return false;

			if ((p + 1 + len) > (*buf + buflen))
				return -1;
			*running_delta = delta + *running_delta;
			option.SetNumber(*running_delta);
			option.SetOption(p + 1, len);
			*buf = p + 1 + len;
			return true;
		}


		int Packet::GetHeaderSize()
		{
			return 4;
		}

		uint8_t Packet::GetVersion()
		{
			return version;
		}

		Thing::CoAP::MessageType Packet::GetType()
		{
			return type;
		}

		Thing::CoAP::Method Packet::GetCode()
		{
			return code;
		}

		uint16_t Packet::GetMessageID()
		{
			return messageid;
		}

		std::vector<uint8_t>& Packet::GetTokens()
		{
			return tokens;
		}

		std::vector<Thing::CoAP::Option>& Packet::GetOptions()
		{
			return options;
		}

		int Packet::GetPayloadLength()
		{
			return payload.size();
		}

		uint8_t* Packet::GetPayload()
		{
			return payload.size() == 0 ? NULL : &payload[0];
		}


		void Packet::SetVersion(uint8_t version)
		{
			this->version = version;
		}

		void Packet::SetType(Thing::CoAP::MessageType type)
		{
			this->type = type;
		}

		void Packet::SetType(uint8_t type)
		{
			this->type = static_cast<Thing::CoAP::MessageType>(type);
		}

		void Packet::SetCode(Thing::CoAP::Method code)
		{
			this->code = code;
		}

		void Packet::SetCode(uint8_t code)
		{
			this->code = static_cast<Thing::CoAP::Method>(code);
		}

		void Packet::SetMessageID(uint16_t messageID)
		{
			this->messageid = messageID;
		}

		void Packet::SetTokens(std::vector<uint8_t> tokens)
		{
			this->tokens = tokens;
		}

		void Packet::SetTokens(uint8_t* tokens, int length)
		{
			this->tokens = std::vector<uint8_t>(tokens, tokens + (int)length);
		}

		void Packet::SetOptions(std::vector<Thing::CoAP::Option> options)
		{
			this->options = options;
		}

		void Packet::ClearOptions()
		{
			this->options.clear();
		}

		void Packet::SetPayload(uint8_t* payload, int length)
		{
			this->payload = std::vector<uint8_t>(payload, payload + length);
		}


#define COAP_OPTION_DELTA(v, n) (v < 13 ? (*n = (0xFF & v)) : (v <= 0xFF + 13 ? (*n = 13) : (*n = 14)))
		void Packet::SerializePacket(uint8_t ** payload, int * length)
		{
			*length = 0;
			*payload = nullptr;

			std::vector<uint8_t>& tokens = GetTokens();
			int size = Packet::GetHeaderSize() + tokens.size() + optionsSize();
			if (GetPayloadLength() > 0)
				size += 1 + GetPayloadLength();

			*length = size;
			*payload = new uint8_t[size];

			uint8_t* buffer = *payload;
			buffer[0] = (GetVersion() << 6) | ((static_cast<uint8_t>(GetType()) & 0x03) << 4) | (tokens.size() & 0x0F);
			buffer[1] = static_cast<uint8_t>(GetCode());
			buffer[2] = GetMessageID() >> 8;
			buffer[3] = (GetMessageID() & 0xFF);

			if (tokens.size() > 0)
				memcpy(&buffer[4], &tokens[0], tokens.size());

			uint8_t *p = buffer + 4 + tokens.size();
			std::vector<Thing::CoAP::Option>& options = GetOptions();
			uint16_t running_delta = 0;
			for (Thing::CoAP::Option& option : options)
			{
				uint32_t optdelta;
				uint8_t len, delta;

				optdelta = static_cast<uint16_t>(option.GetNumber()) - running_delta;
				COAP_OPTION_DELTA(optdelta, &delta);
				COAP_OPTION_DELTA(static_cast<uint32_t>(option.GetLenght()), &len);

				*p++ = (0xFF & (delta << 4 | len));
				if (delta == 13)
					*p++ = (optdelta - 13);
				else if (delta == 14)
				{
					*p++ = ((optdelta - 269) >> 8);
					*p++ = (0xFF & (optdelta - 269));
				}

				if (len == 13)
					*p++ = (option.GetLenght() - 13);
				else if (len == 14)
				{
					*p++ = ((option.GetLenght() - 269) >> 8);
					*p++ = (0xFF & (option.GetLenght() - 269));
				}

				if (option.GetLenght() > 0)
				{
					memcpy(p, option.GetBuffer(), option.GetLenght());
					p += option.GetLenght();
				}
				running_delta = static_cast<uint16_t>(option.GetNumber());
			}

			if (GetPayloadLength() > 0)
			{
				*p++ = 0xFF;
				memcpy(p, GetPayload(), GetPayloadLength());
			}
		}

		void Packet::DesserializePacket(uint8_t* buffer, int length)
		{
			SetVersion((buffer[0] & 0xC0) >> 6);
			SetType((buffer[0] & 0x30) >> 4);
			int tokenLength = buffer[0] & 0x0F;
			SetCode(buffer[1]);
			SetMessageID((0xFF00 & (buffer[2] << 8)) | (0x00FF & buffer[3]));
			SetTokens(&buffer[4], tokenLength);
			//TODO: CoAP protocol defines that no token length should be more than 8. 
			//What should we do here? Protocol says it should be treated as a message format error 

			std::vector<Thing::CoAP::Option> options;
			if (Packet::GetHeaderSize() + tokenLength < length)
			{
				uint32_t delta = 0;
				uint8_t *end = buffer + length;
				uint8_t *p = buffer + Packet::GetHeaderSize() + tokenLength;

				while (*p != 0xFF && p < end)
				{
					Thing::CoAP::Option option;
					if (GetOption(option, &delta, &p, end - p))
						options.push_back(option);
				}
				SetOptions(options);

				if (p + 1 < end && *p == 0xFF)
					SetPayload(p + 1, end - (p + 1));
				else
					SetPayload(NULL, 0);
			}
		}

		int Packet::optionsSize()
		{
			int totalbytes = 0;
			std::vector<Thing::CoAP::Option>& options = GetOptions();
			uint16_t running_delta = 0;
			for (Thing::CoAP::Option& option : options)
			{
				uint32_t optdelta = static_cast<uint32_t>(option.GetNumber()) - running_delta;
				if (13 <= optdelta && optdelta < 269)
					totalbytes += 1;
				else if (269 < optdelta)
					totalbytes += 2;

				int optionSize = option.GetLenght();
				if (13 <= optionSize && optionSize < 269)
					totalbytes += 1;
				else if (269 < optionSize)
					totalbytes += 2;

				totalbytes += optionSize + 1;
				running_delta = static_cast<uint16_t>(option.GetNumber());
			}
			return totalbytes;
		}
	}
}