#include <iostream>
#include "Thing.CoAP/Client.h"
#include "Thing.CoAP/Linux/UDPPacketProvider.h"
#include <thread>
#include <stdlib.h>
#include "Thing.CoAP/WebLink.h"

int main()
{
	//Declare our CoAP client instance and packet handler
	Thing::CoAP::Client coapClient;

	//Tell our client that we will be using UDP 
	Thing::CoAP::Linux::UDPPacketProvider<1000> packetProvider;
	coapClient.SetPacketProvider(packetProvider);

	//Connect CoAP client to a server
	coapClient.Start(inet_addr("127.0.0.1"), 5683);

	//Make the discovery
	coapClient.Discover([](Thing::CoAP::Response& response) {
		std::vector<uint8_t> payload = response.GetPayload();
		std::string received(payload.begin(), payload.end());

		//Parse the response to weblinks
		std::list<Thing::CoAP::WebLink> links = Thing::CoAP::WebLink::ParseString(received);
		for (auto& link : links)
		{
			//Print the URI of the resource
			std::cout << link.GetURI() << std::endl;
			//Prints if the resource is observable or not
			std::cout << '\t' << "Observable: " << (link.IsObservable() ? " Yes" : "No") << std::endl;

			//Prints the content format of the resource
			std::cout << '\t' << "Content Format: ";
			switch (link.GetContentFormat())
			{
			case Thing::CoAP::ContentFormat::ApplicationEXI: std::cout << "ApplicationEXI" << std::endl; break;
			case Thing::CoAP::ContentFormat::ApplicationJSon: std::cout << "ApplicationJSon" << std::endl; break;
			case Thing::CoAP::ContentFormat::ApplicationLinkFormat: std::cout << "ApplicationLinkFormat" << std::endl; break;
			case Thing::CoAP::ContentFormat::ApplicationOctetStream: std::cout << "ApplicationOctetStream" << std::endl; break;
			case Thing::CoAP::ContentFormat::ApplicationXML: std::cout << "ApplicationXML" << std::endl; break;
			case Thing::CoAP::ContentFormat::TextPlain: std::cout << "TextPlain" << std::endl; break;
			}

			//If the link has title, print the title
			if (link.HasTitle())
				std::cout << '\t' << "Title: " << link.GetTitle() << std::endl;
			//If the link has Interface Description, print the Interface Description
			if (link.HasInterfaceDescription())
				std::cout << '\t' << "Interface Description: " << link.GetInterfaceDescription() << std::endl;
			//If the link has Maximum Size Estimate, print the Maximum Size Estimate
			if (link.HasMaximumSizeEstimate())
				std::cout << '\t' << "Maximum Size Estimate: " << link.GetMaximumSizeEstimate() << std::endl;
			//If the link has Resource Type, print the Resource Type
			if (link.HasResourceType())
				std::cout << '\t' << "Resource Type: " << link.GetResourceType() << std::endl;
		}
		exit(0);
	});

	//Keep processing the data forever
	for (;;)
		coapClient.Process();

	coapClient.Stop();
}