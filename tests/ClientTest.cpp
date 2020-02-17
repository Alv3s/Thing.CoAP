#include "ClientTest.h"

#include "../Thing.CoAP/Client.h"
#include "PacketProviderMock.h"
#include "Common.h"

using ::testing::_;
using ::testing::SaveArg;
using ::testing::Contains;
using ::testing::SetArgPointee;
using ::testing::Return;

namespace Thing {
	namespace CoAP {
		namespace Tests {
			void ClientTest::SetUp()
			{
			}

			void ClientTest::TearDown()
			{
			}

			TEST_F(ClientTest, StartAndSetPort)
			{
				const int port = 9999;

				Client coapClient;
				coapClient.SetPort(port);

				PacketProviderMock packetProviderMock;
				EXPECT_CALL(packetProviderMock, Start(port)).Times(1);

				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(0, 0);
			}

			TEST_F(ClientTest, Discover)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				const std::string endpoint = ".well-known/core";

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				coapClient.Discover([&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Get, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(0, request.GetPayload().size());

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, GetEmpty)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				coapClient.Get(endpoint, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Get, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(0, request.GetPayload().size());

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, GetWithString)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";
				std::string payload = "SomePayload";

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response& response)> callbackMock;
				coapClient.Get(endpoint, payload, [&callbackMock](Thing::CoAP::Response& response) 
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Get, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(payload.size(), request.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(&payload[0], &request.GetPayload()[0], payload.size()));

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, GetWithVector)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";
				std::vector<uint8_t> payload = {0x00, 0x01, 0x02, 0x03};

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				coapClient.Get(endpoint, payload, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Get, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(payload.size(), request.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(&payload[0], &request.GetPayload()[0], payload.size()));

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, PostWithString)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";
				std::string payload = "SomePayload";

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				coapClient.Post(endpoint, payload, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Post, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(payload.size(), request.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(&payload[0], &request.GetPayload()[0], payload.size()));

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, PostWithVector)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";
				std::vector<uint8_t> payload = { 0x00, 0x01, 0x02, 0x03 };

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				coapClient.Post(endpoint, payload, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Post, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(payload.size(), request.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(&payload[0], &request.GetPayload()[0], payload.size()));

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, PutWithString)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";
				std::string payload = "SomePayload";

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				coapClient.Put(endpoint, payload, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Put, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(payload.size(), request.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(&payload[0], &request.GetPayload()[0], payload.size()));

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, PutWithVector)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";
				std::vector<uint8_t> payload = { 0x00, 0x01, 0x02, 0x03 };

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				coapClient.Put(endpoint, payload, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Put, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(payload.size(), request.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(&payload[0], &request.GetPayload()[0], payload.size()));

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, DeleteWithString)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";
				std::string payload = "SomePayload";

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				coapClient.Delete(endpoint, payload, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Delete, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(payload.size(), request.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(&payload[0], &request.GetPayload()[0], payload.size()));

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, DeleteWithVector)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";
				std::vector<uint8_t> payload = { 0x00, 0x01, 0x02, 0x03 };

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				coapClient.Delete(endpoint, payload, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Delete, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				ASSERT_EQ(payload.size(), request.GetPayload().size());
				EXPECT_TRUE(0 == std::memcmp(&payload[0], &request.GetPayload()[0], payload.size()));

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, Observe)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(1).WillOnce(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				auto token = coapClient.Observe(endpoint, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});

				EXPECT_EQ(endpoint, token.GetEndpoint());

				Request request;
				request.DesserializePacket(networkPacket);

				uint16_t tokenID = token.GetToken();
				uint8_t tokens[2] = { (tokenID >> 8) & 0xFF, tokenID & 0xFF };
				EXPECT_EQ(Method::Get, request.GetCode());
				EXPECT_EQ(2, request.GetTokens().size());
				EXPECT_TRUE(0 == std::memcmp(&tokens[0], &request.GetTokens()[0], 2));
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::Observe, (uint8_t*)NULL, 0)));
				ASSERT_EQ(0, request.GetPayload().size());

				//It's enough to return the same sent packet as fot his test, we only want to make sure the callback is called and for that, the sent token just need to match the incoming token
				EXPECT_CALL(packetProviderMock, ReadPacket(_, _, _)).Times(1).WillOnce(DoAll(SetArgPointee<0>(networkPacket), Return(true)));
				EXPECT_CALL(callbackMock, Call(_)).Times(1);

				coapClient.Process();
			}

			TEST_F(ClientTest, ObserveCancel)
			{
				const IPAddress ip = 0x10203040;
				const int port = 9999;
				std::string endpoint = "SomeEndpoint";
				const uint8_t observeOption = 1;

				Client coapClient;
				PacketProviderMock packetProviderMock;
				coapClient.SetPacketProvider(packetProviderMock);
				coapClient.Start(ip, port);

				std::vector<uint8_t> networkPacket;
				EXPECT_CALL(packetProviderMock, SendPacket(_, ip, port)).Times(2).WillRepeatedly(SaveArg<0>(&networkPacket));

				testing::MockFunction<void(Thing::CoAP::Response & response)> callbackMock;
				auto token = coapClient.Observe(endpoint, [&callbackMock](Thing::CoAP::Response& response)
					{
						callbackMock.Call(response);
					});


				coapClient.CancelObserve(token);

				Request request;
				request.DesserializePacket(networkPacket);

				EXPECT_EQ(Method::Get, request.GetCode());
				EXPECT_EQ(Thing::CoAP::MessageType::NonConfirmable, request.GetType());
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::URIPath, (uint8_t*)endpoint.c_str(), endpoint.size())));
				EXPECT_THAT(request.GetOptions(), Contains(Option(Thing::CoAP::OptionValue::Observe, (uint8_t*)&observeOption, 1)));
				ASSERT_EQ(0, request.GetPayload().size());
			}
		}
	}
}