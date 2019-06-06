#include "PacketTest.h"
#include "../Thing.CoAP/Packet.h"
#include <cstring>
#include <vector>

namespace Thing {
	namespace CoAP {
		namespace Tests {
			void PacketTest::SetUp()
			{

			}

			void PacketTest::TearDown()
			{

			}

			TEST_F(PacketTest, PacketObjectFill)
			{
				Thing::CoAP::Packet packet;

				const uint8_t code = 4;
				const uint8_t type = 1;
				const uint8_t version = 2;
				const uint16_t messageId = 8;
				const uint8_t payload[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				const uint32_t tokens[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
				packet.SetCode(code);
				packet.SetMessageID(messageId);
				packet.SetPayload((uint8_t*)payload, sizeof(payload));
				packet.SetTokens((uint8_t*)tokens, sizeof(tokens));
				packet.SetType(type);
				packet.SetVersion(version);

				const int totalOptions = 5;
				const uint8_t optionsBuffer[] = { 0, 1, 2, 3, 4 };
				Thing::CoAP::Option options[totalOptions];
				for (int i = 0; i < totalOptions; ++i)
				{
					options[i].SetNumber(i);
					options[i].SetOption((uint8_t*)optionsBuffer, sizeof(optionsBuffer));
				}

				EXPECT_EQ(code, static_cast<uint8_t>(packet.GetCode()));
				EXPECT_EQ(type, static_cast<uint8_t>(packet.GetType()));
				EXPECT_EQ(version, packet.GetVersion());
				EXPECT_EQ(messageId, packet.GetMessageID());

				EXPECT_EQ(sizeof(payload), packet.GetPayload().size());
				EXPECT_EQ(0, memcmp(payload, &packet.GetPayload()[0], sizeof(payload)));

				EXPECT_EQ(sizeof(tokens), packet.GetTokens().size());
				EXPECT_EQ(0, memcmp(tokens, &packet.GetTokens()[0], sizeof(tokens)));

				std::vector<Thing::CoAP::Option> optionsP = packet.GetOptions();
				for (int i = 0; i < optionsP.size(); ++i)
				{
					EXPECT_EQ(i, static_cast<int>(optionsP[i].GetNumber()));
					EXPECT_EQ(sizeof(optionsBuffer), optionsP[i].GetLenght());
					EXPECT_EQ(0, memcmp(optionsBuffer, optionsP[i].GetBuffer(), sizeof(optionsBuffer)));
				}
			}

			TEST_F(PacketTest, DesserializeTest)
			{
				const uint8_t version = 1;
				const uint8_t type = static_cast<uint8_t>(Thing::CoAP::MessageType::Reset);
				const uint8_t code = static_cast<uint8_t>(Thing::CoAP::Method::Get);
				const uint16_t transactionID = 0xFE0F;
				const uint8_t tokens[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
				const uint8_t option1[] = { 0x13, 0x01, 0xA1, 0x01 }; //Option Number 0 + 1 = 1
				const uint8_t option2[] = { 0xD3, 0x01, 0x02, 0xB2, 0x02 }; //Option Number 1 + (13+1) = 15
				const uint8_t option3[] = { 0xDD, 0x02, 0x01, 0x03, 0xC3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }; //Option Number 15 + (13 + 2) = 30
				const uint8_t option4[] = { 0xE3, 0x00, 0xA0, 0x04, 0xD4, 0x04 }; //Option Number = 30 + (269 + 160) = 459
				const uint8_t option5[] = { 0xED, 0x0B, 0xA0, 0x01, 0x03, 0xC3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }; //Option Number 459 + (269 + 2976) = 3704
				const uint8_t option6[5 + 270] = { 0xEE, 0x00, 0x01, 0x00, 0x01 }; //Option Number 3704 + (269 + 1) = 3974
				const uint8_t* options[] = { option1, option2, option3, option4, option5, option6 };
				const uint8_t payloadMarker = 0xFF;
				const uint8_t payload[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A };

				int size = Thing::CoAP::Packet::GetHeaderSize() +
					sizeof(tokens) +
					sizeof(option1) +
					sizeof(option2) +
					sizeof(option3) +
					sizeof(option4) +
					sizeof(option5) +
					sizeof(option6) +
					sizeof(uint8_t) +
					sizeof(payload);
				uint8_t* buffer = new uint8_t[size];
				buffer[0] = (0b11000000 & (version << 6)) | (0b00110000 & (type << 4)) | (0b00001111 & sizeof(tokens));
				buffer[1] = code;
				buffer[2] = 0xFF & (transactionID >> 8);
				buffer[3] = 0xFF & transactionID;
				std::memcpy(&buffer[4], tokens, sizeof(tokens));
				std::memcpy(&buffer[4 + sizeof(tokens)], option1, sizeof(option1));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1)], option2, sizeof(option2));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2)], option3, sizeof(option3));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3)], option4, sizeof(option4));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3) + sizeof(option4)], option5, sizeof(option5));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3) + sizeof(option4) + sizeof(option5)], option6, sizeof(option6));
				buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3) + sizeof(option4) + sizeof(option5) + sizeof(option6)] = payloadMarker;
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3) + sizeof(option4) + sizeof(option5) + sizeof(option6) + 1], payload, sizeof(payload));

				Thing::CoAP::Packet p;
				p.DesserializePacket(buffer, size);

				EXPECT_EQ(version, static_cast<uint8_t>(p.GetVersion()));
				EXPECT_EQ(type, static_cast<uint8_t>(p.GetType()));
				EXPECT_EQ(code, static_cast<uint8_t>(p.GetCode()));
				EXPECT_EQ(transactionID, static_cast<uint16_t>(p.GetMessageID()));

				EXPECT_EQ(sizeof(tokens), p.GetTokens().size());
				EXPECT_TRUE(0 == std::memcmp(tokens, &p.GetTokens()[0], sizeof(tokens)));

				std::vector<Thing::CoAP::Option>& pOptions = p.GetOptions();
				EXPECT_EQ(6, pOptions.size());
				int runningDelta = 0;
				for (int i = 0; i < pOptions.size(); ++i)
				{
					Thing::CoAP::Option option = pOptions[i];

					int currentDelta = 0 | ((options[i][0] >> 4) & 0x0F);
					uint16_t currentLength = options[i][0] & 0x0F;

					uint8_t* aux = ((uint8_t*)options[i]) + 1;

					if (currentDelta == 13)
					{
						currentDelta = *aux + 13;
						aux += 1;
					}
					else if (currentDelta == 14)
					{
						currentDelta = (((aux[0] << 8) & 0xFF00) | (aux[1] & 0x00FF)) + (int)269;
						aux += 2;
					}

					if (currentLength == 13)
					{
						currentLength = *aux + 13;
						aux += 1;
					}
					else if (currentLength == 14)
					{
						currentLength = (((aux[0] << 8) & 0xFF00) | (aux[1] & 0x00FF)) + (int)269;
						aux += 2;
					}

					int expectedOptionNumber = runningDelta + currentDelta;
					int expectedOptionLength = currentLength;

					runningDelta = expectedOptionNumber;

					EXPECT_EQ(expectedOptionLength, option.GetLenght());
					EXPECT_EQ(static_cast<Thing::CoAP::OptionValue>(expectedOptionNumber), option.GetNumber());
					EXPECT_TRUE(0 == std::memcmp(aux, option.GetBuffer(), option.GetLenght()));
				}

				EXPECT_EQ(sizeof(payload), p.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(payload, &p.GetPayload()[0], sizeof(payload)));
				delete[] buffer;
			}

			TEST_F(PacketTest, DesserializeTestViaVector)
			{
				const uint8_t version = 1;
				const uint8_t type = static_cast<uint8_t>(Thing::CoAP::MessageType::Reset);
				const uint8_t code = static_cast<uint8_t>(Thing::CoAP::Method::Get);
				const uint16_t transactionID = 0xFE0F;
				const uint8_t tokens[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
				const uint8_t option1[] = { 0x13, 0x01, 0xA1, 0x01 }; //Option Number 0 + 1 = 1
				const uint8_t option2[] = { 0xD3, 0x01, 0x02, 0xB2, 0x02 }; //Option Number 1 + (13+1) = 15
				const uint8_t option3[] = { 0xDD, 0x02, 0x01, 0x03, 0xC3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }; //Option Number 15 + (13 + 2) = 30
				const uint8_t option4[] = { 0xE3, 0x00, 0xA0, 0x04, 0xD4, 0x04 }; //Option Number = 30 + (269 + 160) = 459
				const uint8_t option5[] = { 0xED, 0x0B, 0xA0, 0x01, 0x03, 0xC3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }; //Option Number 459 + (269 + 2976) = 3704
				const uint8_t option6[5 + 270] = { 0xEE, 0x00, 0x01, 0x00, 0x01 }; //Option Number 3704 + (269 + 1) = 3974
				const uint8_t* options[] = { option1, option2, option3, option4, option5, option6 };
				const uint8_t payloadMarker = 0xFF;
				const uint8_t payload[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A };

				int size = Thing::CoAP::Packet::GetHeaderSize() +
					sizeof(tokens) +
					sizeof(option1) +
					sizeof(option2) +
					sizeof(option3) +
					sizeof(option4) +
					sizeof(option5) +
					sizeof(option6) +
					sizeof(uint8_t) +
					sizeof(payload);
				uint8_t* buffer = new uint8_t[size];
				buffer[0] = (0b11000000 & (version << 6)) | (0b00110000 & (type << 4)) | (0b00001111 & sizeof(tokens));
				buffer[1] = code;
				buffer[2] = 0xFF & (transactionID >> 8);
				buffer[3] = 0xFF & transactionID;
				std::memcpy(&buffer[4], tokens, sizeof(tokens));
				std::memcpy(&buffer[4 + sizeof(tokens)], option1, sizeof(option1));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1)], option2, sizeof(option2));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2)], option3, sizeof(option3));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3)], option4, sizeof(option4));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3) + sizeof(option4)], option5, sizeof(option5));
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3) + sizeof(option4) + sizeof(option5)], option6, sizeof(option6));
				buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3) + sizeof(option4) + sizeof(option5) + sizeof(option6)] = payloadMarker;
				std::memcpy(&buffer[4 + sizeof(tokens) + sizeof(option1) + sizeof(option2) + sizeof(option3) + sizeof(option4) + sizeof(option5) + sizeof(option6) + 1], payload, sizeof(payload));

				Thing::CoAP::Packet p;
				p.DesserializePacket(std::vector<uint8_t>(buffer, buffer + size));

				EXPECT_EQ(version, static_cast<uint8_t>(p.GetVersion()));
				EXPECT_EQ(type, static_cast<uint8_t>(p.GetType()));
				EXPECT_EQ(code, static_cast<uint8_t>(p.GetCode()));
				EXPECT_EQ(transactionID, static_cast<uint16_t>(p.GetMessageID()));

				EXPECT_EQ(sizeof(tokens), p.GetTokens().size());
				EXPECT_TRUE(0 == std::memcmp(tokens, &p.GetTokens()[0], sizeof(tokens)));

				std::vector<Thing::CoAP::Option>& pOptions = p.GetOptions();
				EXPECT_EQ(6, pOptions.size());
				int runningDelta = 0;
				for (int i = 0; i < pOptions.size(); ++i)
				{
					Thing::CoAP::Option option = pOptions[i];

					int currentDelta = 0 | ((options[i][0] >> 4) & 0x0F);
					uint16_t currentLength = options[i][0] & 0x0F;

					uint8_t* aux = ((uint8_t*)options[i]) + 1;

					if (currentDelta == 13)
					{
						currentDelta = *aux + 13;
						aux += 1;
					}
					else if (currentDelta == 14)
					{
						currentDelta = (((aux[0] << 8) & 0xFF00) | (aux[1] & 0x00FF)) + (int)269;
						aux += 2;
					}

					if (currentLength == 13)
					{
						currentLength = *aux + 13;
						aux += 1;
					}
					else if (currentLength == 14)
					{
						currentLength = (((aux[0] << 8) & 0xFF00) | (aux[1] & 0x00FF)) + (int)269;
						aux += 2;
					}

					int expectedOptionNumber = runningDelta + currentDelta;
					int expectedOptionLength = currentLength;

					runningDelta = expectedOptionNumber;

					EXPECT_EQ(expectedOptionLength, option.GetLenght());
					EXPECT_EQ(static_cast<Thing::CoAP::OptionValue>(expectedOptionNumber), option.GetNumber());
					EXPECT_TRUE(0 == std::memcmp(aux, option.GetBuffer(), option.GetLenght()));
				}

				EXPECT_EQ(sizeof(payload), p.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(payload, &p.GetPayload()[0], sizeof(payload)));
				delete[] buffer;
			}

			TEST_F(PacketTest, SerializeTest)
			{
				const uint8_t version = 1;
				const uint8_t type = static_cast<uint8_t>(Thing::CoAP::MessageType::Reset);
				const uint8_t code = static_cast<uint8_t>(Thing::CoAP::Method::Get);
				const uint16_t transactionID = 0xFE0F;
				const uint8_t tokens[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
				const uint8_t option1[] = { 0x01, 0xA1, 0x01 }; //Option Number 0 + 1 = 1
				const uint8_t option2[] = { 0x02, 0xB2, 0x02 }; //Option Number 1 + (13+1) = 15
				const uint8_t option3[] = { 0x03, 0xC3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }; //Option Number 15 + (13 + 2) = 30
				const uint8_t option4[] = { 0x04, 0xD4, 0x04 }; //Option Number = 30 + (269 + 160) = 459
				const uint8_t option5[] = { 0x03, 0xC3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }; //Option Number 459 + (269 + 2976) = 3704
				const uint8_t option6[270] = {};
				const int optionsSize[] = { sizeof(option1), sizeof(option2), sizeof(option3), sizeof(option4), sizeof(option5), sizeof(option6) };
				const uint8_t* options[] = { option1, option2, option3, option4, option5, option6 };
				const uint8_t payloadMarker = 0xFF;
				const uint8_t payload[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A };

				std::memset((uint8_t*)option6, 0x01, sizeof(option6));

				Thing::CoAP::Packet packet;
				packet.SetVersion(version);
				packet.SetType(type);
				packet.SetCode(code);
				packet.SetMessageID(transactionID);
				std::vector<uint8_t> t(tokens, tokens + sizeof(tokens));
				packet.SetTokens(t);

				Thing::CoAP::Option o1;
				o1.SetNumber(1);
				o1.SetOption((uint8_t*)option1, sizeof(option1));

				Thing::CoAP::Option o2;
				o2.SetNumber(15);
				o2.SetOption((uint8_t*)option2, sizeof(option2));

				Thing::CoAP::Option o3;
				o3.SetNumber(30);
				o3.SetOption((uint8_t*)option3, sizeof(option3));

				Thing::CoAP::Option o4;
				o4.SetNumber(459);
				o4.SetOption((uint8_t*)option4, sizeof(option4));

				Thing::CoAP::Option o5;
				o5.SetNumber(3704);
				o5.SetOption((uint8_t*)option5, sizeof(option5));

				Thing::CoAP::Option o6;
				o6.SetNumber(3974);
				o6.SetOption((uint8_t*)option6, sizeof(option6));

				std::vector<Thing::CoAP::Option> pOptions;
				pOptions.push_back(o1);
				pOptions.push_back(o2);
				pOptions.push_back(o3);
				pOptions.push_back(o4);
				pOptions.push_back(o5);
				pOptions.push_back(o6);
				packet.SetOptions(pOptions);

				packet.SetPayload((uint8_t*)payload, sizeof(payload));

				std::vector<uint8_t> buffer = packet.SerializePacket();

				EXPECT_EQ(version, (buffer[0] >> 6) & 0b00000011);
				EXPECT_EQ(type, (buffer[0] >> 4) & 0b00000011);
				EXPECT_EQ(sizeof(tokens), buffer[0] & 0b00001111);
				EXPECT_EQ(code, buffer[1]);
				EXPECT_EQ(transactionID, ((buffer[2] << 8) & 0xFF00) | (buffer[3] & 0xFF));
				EXPECT_TRUE(0 == std::memcmp((uint8_t*)tokens, &buffer[4], sizeof(tokens)));

				int runningDelta = 0;
				uint8_t* p = &buffer[4 + sizeof(tokens)];
				for (int i = 0; i < pOptions.size(); ++i)
				{
					int currentDelta = 0 | ((p[0] >> 4) & 0x0F);
					uint16_t currentLength = p[0] & 0x0F;

					uint8_t* aux = p + 1;

					if (currentDelta == 13)
					{
						currentDelta = *aux + 13;
						aux += 1;
					}
					else if (currentDelta == 14)
					{
						currentDelta = (((aux[0] << 8) & 0xFF00) | (aux[1] & 0x00FF)) + (int)269;
						aux += 2;
					}

					if (currentLength == 13)
					{
						currentLength = *aux + 13;
						aux += 1;
					}
					else if (currentLength == 14)
					{
						currentLength = (((aux[0] << 8) & 0xFF00) | (aux[1] & 0x00FF)) + (int)269;
						aux += 2;
					}

					int optionNumber = runningDelta + currentDelta;
					int optionLength = currentLength;

					runningDelta = optionNumber;

					Thing::CoAP::Option option = pOptions[i];
					EXPECT_EQ(option.GetLenght(), optionLength);
					EXPECT_EQ(static_cast<uint16_t>(option.GetNumber()), optionNumber);
					EXPECT_TRUE(0 == std::memcmp(aux, option.GetBuffer(), option.GetLenght()));

					p = aux + option.GetLenght();
				}

				EXPECT_EQ(0xFF, p[0]);
				EXPECT_TRUE(0 == std::memcmp(&p[1], payload, sizeof(payload)));
			}

			TEST_F(PacketTest, NoPayloadNoCrash)
			{
				const uint8_t version = 1;
				const uint8_t type = static_cast<uint8_t>(Thing::CoAP::MessageType::Reset);
				const uint8_t code = static_cast<uint8_t>(Thing::CoAP::Method::Get);
				const uint16_t transactionID = 0xFE0F;
				const uint8_t tokens[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
				const uint8_t option[] = { 0x01, 0xA1, 0x01 }; //Option Number 0 + 1 = 1

				Thing::CoAP::Packet packet;
				packet.SetVersion(version);
				packet.SetType(type);
				packet.SetCode(code);
				packet.SetMessageID(transactionID);
				std::vector<uint8_t> t(tokens, tokens + sizeof(tokens));
				packet.SetTokens(t);

				Thing::CoAP::Option o;
				o.SetNumber(1);
				o.SetOption((uint8_t*)option, sizeof(option));

				std::vector<Thing::CoAP::Option> options;
				options.push_back(o);
				packet.SetOptions(options);

				packet.SetPayload(NULL, 0);

				EXPECT_EQ(0, packet.GetPayload().size());
			}
		
			TEST_F(PacketTest, SetPayload)
			{
				uint8_t buffer[] = { 0x01, 0x02, 0x03 };
				Thing::CoAP::Packet packet;
				packet.SetPayload(buffer, sizeof(buffer));

				ASSERT_EQ(sizeof(buffer), packet.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(buffer, &packet.GetPayload()[0], sizeof(buffer)));
			}

			TEST_F(PacketTest, SetPayloadViaVector)
			{
				uint8_t buffer[] = { 0x01, 0x02, 0x03 };
				Thing::CoAP::Packet packet;
				packet.SetPayload(std::vector<uint8_t>(buffer, buffer + sizeof(buffer)));

				ASSERT_EQ(sizeof(buffer), packet.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(buffer, &packet.GetPayload()[0], sizeof(buffer)));
			}
		}
	}
}