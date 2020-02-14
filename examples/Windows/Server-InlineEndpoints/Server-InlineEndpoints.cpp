#include <iostream>

#include "Thing.CoAP/Server.h"
#include "Thing.CoAP/Windows/UDPPacketProvider.h"
#include <thread>
#include <stdlib.h>

int main()
{
	//Declare our CoAP server instance
	Thing::CoAP::Server server;

	//Tell our server that we will be using UDP 
	Thing::CoAP::Windows::UDPPacketProvider<1000> packetProvider;
	server.SetPacketProvider(packetProvider);

	//Create an "hello world" endpoint
	auto endpoint = &server.CreateResource("helloworld", Thing::CoAP::ContentFormat::TextPlain, false)
		.OnGet([](Thing::CoAP::Request& request) {
			std::vector<uint8_t> payload = request.GetPayload();

			std::string received(payload.begin(), payload.end());
			std::cout << "Client sent: " << received << std::endl;

			return Thing::CoAP::Status::Ok("Hello World!");
		}).OnPost([](Thing::CoAP::Request& request) {
			std::vector<uint8_t> payload = request.GetPayload();

			std::string received(payload.begin(), payload.end());
			std::cout << "Client sent: " << received << std::endl;

			return Thing::CoAP::Status::Ok("Hello From The Server! The message you sent was:" + received);
		});

		server.Start();

		std::thread t([&endpoint]() {
			for (;;)
			{
				int randomSleep = rand() % 5000 + 1000;
				endpoint->ObservableChanged(Thing::CoAP::Status::Ok("Hello Clients! Next message is going to be sent in " + std::to_string(randomSleep) + "ms"));
				Sleep(randomSleep);
			}
		});

		for (;;)
			server.Process();
		return 0;
}