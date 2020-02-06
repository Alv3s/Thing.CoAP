#include <iostream>
#include "Thing.CoAP/Client.h"
#include "Thing.CoAP/Windows/UDPPacketProvider.h"
#include <thread>


void sendMessage(Thing::CoAP::Client& coapClient) {
	//Make a post
	coapClient.Get("create1", "", [&coapClient](Thing::CoAP::Response response) {
		std::vector<uint8_t> payload = response.GetPayload();
		std::string received(payload.begin(), payload.end());

		std::cout << "Server sent the following message:" << std::endl;
		std::cout << received << std::endl;

		Sleep(5000);
		sendMessage(coapClient);
	});
}

int main()
{
	//Declare our CoAP client instance and packet handler
	Thing::CoAP::Client coapClient;

	//Tell our client that we will be using UDP 
	Thing::CoAP::Windows::UDPPacketProvider<1000> packetProvider;
	coapClient.SetPacketProvider(packetProvider);

	//Connect CoAP client to a server
	coapClient.Start(inet_addr("104.196.15.150"), 5683);

	//Send the message
	sendMessage(coapClient);

	//Keep processing the data forever
	for (;;)
		coapClient.Process();

	coapClient.Stop();
}