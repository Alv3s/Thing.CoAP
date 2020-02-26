#include <iostream>
#include "Thing.CoAP/Server.h"
#include "Thing.CoAP/Linux/UDPPacketProvider.h"
#include <thread>
#include <stdlib.h>

int main(int argc, char **argv)
{
	//Declare our CoAP server instance
	Thing::CoAP::Server server;

	//Tell our server that we will be using UDP 
	Thing::CoAP::Linux::UDPPacketProvider<1000> packetProvider;
	server.SetPacketProvider(packetProvider);

	//Create an "hello world" endpoint
	auto endpoint = &server.CreateResource("helloworld", Thing::CoAP::ContentFormat::TextPlain, true)
		.OnGet([](Thing::CoAP::Request& request) {
			std::vector<uint8_t> payload = request.GetPayload();

			std::string received(payload.begin(), payload.end());
			std::cout << "Client sent: " << received << std::endl;

			return Thing::CoAP::Status::Content("Hello World!");
		}).OnPost([](Thing::CoAP::Request& request) {
			std::vector<uint8_t> payload = request.GetPayload();

			std::string received(payload.begin(), payload.end());
			std::cout << "Client sent: " << received << std::endl;

			return Thing::CoAP::Status::Content("Hello From The Server! The message you sent was:" + received);
		});

		server.Start();

		std::thread t([&endpoint]() {
			for (;;)
			{
				int randomSleep = rand() % 5000 + 1000;
				endpoint->ObservableChanged(Thing::CoAP::Status::Content("Hello Clients! Next message is going to be sent in " + std::to_string(randomSleep) + "ms"));
				usleep(randomSleep * 1000);
			}
		});

		for (;;)
			server.Process();
		return 0;
}