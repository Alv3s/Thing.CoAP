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

			std::string string_data = "Hello world!";
			uint8_t* data = (uint8_t*)string_data.c_str();
			size_t data_length = string_data.length()+1;

			return Thing::CoAP::Status::Content(data, data_length);
		}).OnPost([](Thing::CoAP::Request& request) {
			std::vector<uint8_t> payload = request.GetPayload();

			std::string received(payload.begin(), payload.end());
			std::cout << "Client sent: " << received << std::endl;

			std::string string_data = "Hello From The Server! The message you sent was:" + received;
			uint8_t* data = (uint8_t*)string_data.c_str();
			size_t data_length = string_data.length()+1;

			return Thing::CoAP::Status::Content(data, data_length);
		});

		server.Start();

		std::thread t([&endpoint]() {
			for (;;)
			{
				int randomSleep = rand() % 5000 + 1000;

				std::string string_data = "Hello Clients! Next message is going to be sent in " + std::to_string(randomSleep) + "ms";
				uint8_t* data = (uint8_t*)string_data.c_str();
				size_t data_length = string_data.length()+1;

				endpoint->ObservableChanged(Thing::CoAP::Status::Content(data, data_length));
				usleep(randomSleep * 1000);
			}
		});

		for (;;)
			server.Process();
		return 0;
}