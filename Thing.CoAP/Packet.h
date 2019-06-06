#pragma once

#include <cstdint>
#include <vector>
#include "Option.h"
#include "Method.h"
#include "MessageType.h"

namespace Thing {
	namespace CoAP
	{
		class Packet
		{
		public:
			static int GetHeaderSize();

			uint8_t GetVersion();
			Thing::CoAP::MessageType GetType();
			Thing::CoAP::Method GetCode();
			uint16_t GetMessageID();
			std::vector<uint8_t>& GetTokens();
			std::vector<Thing::CoAP::Option>& GetOptions();
			std::vector<uint8_t> GetPayload();


			void SetVersion(uint8_t version);

			void SetType(Thing::CoAP::MessageType type);
			void SetType(uint8_t type);

			void SetCode(Thing::CoAP::Method code);
			void SetCode(uint8_t code);

			void SetMessageID(uint16_t messageID);

			void SetTokens(std::vector<uint8_t> tokens);
			void SetTokens(uint8_t* tokens, int length);

			void SetOptions(std::vector<Thing::CoAP::Option> options);
			void ClearOptions();
			void SetPayload(uint8_t* payload, int length);
			void SetPayload(std::vector<uint8_t> payload);

			std::vector<uint8_t> SerializePacket();
			void DesserializePacket(uint8_t* payload, int length);
			void DesserializePacket(std::vector<uint8_t> payload);
		private:
			uint8_t version;
			Thing::CoAP::MessageType type;
			Thing::CoAP::Method code;
			std::vector<uint8_t> tokens;
			std::vector<uint8_t> payload;
			uint16_t messageid;
			std::vector<Thing::CoAP::Option> options;

			int optionsSize();
		};
	}
}