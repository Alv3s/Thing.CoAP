#include <WiFi.h>

//Include Thing.CoAP
#include "Thing.CoAP.h"

//[RECOMMENDED] Alternatively, if you are NOT using Arduino IDE you can include each file you need as bellow: 
//#include "Thing.CoAP/Client.h"
//#include "Thing.CoAP/ESP/UDPPacketProvider.h"
//#include "Thing.CoAP/WebLink.h"

//Declare our CoAP client and the packet handler
Thing::CoAP::Client coapClient;
Thing::CoAP::ESP::UDPPacketProvider udpProvider;

//Change here your WiFi settings
char* ssid = "YourWiFiSSID";
char* password = "YourWiFiPassword";

void setup() {
	//Initializing the Serial
	Serial.begin(115200);
	Serial.println("Initializing");

	//Try and wait until a connection to WiFi was made
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}
	Serial.println("Connected to the WiFi network");
	Serial.println("My IP: ");
	Serial.println(WiFi.localIP());

	//Configure our server to use our packet handler (It will use UDP)
	coapClient.SetPacketProvider(udpProvider);
	IPAddress ip(104, 196, 15, 150);

	//Connect CoAP client to a server
	coapClient.Start(ip, 5683);
	
	//Make the discovery
	coapClient.Discover([](Thing::CoAP::Response& response) {
		std::vector<uint8_t> payload = response.GetPayload();
		std::string received(payload.begin(), payload.end());

		//Parse the response to weblinks
		std::list<Thing::CoAP::WebLink> links = Thing::CoAP::WebLink::ParseString(received);
		for (auto& link : links)
		{
			//Print the URI of the resource
			Serial.println(link.GetURI().c_str());
			
			//Prints if the resource is observable or not
			Serial.print("\t");
			Serial.print("Observable: ");
			Serial.println((link.IsObservable() ? " Yes" : "No"));
			
			//Prints the content format of the resource
			Serial.print("\t");
			Serial.print("Content Format: ");
			switch (link.GetContentFormat())
			{
			case Thing::CoAP::ContentFormat::ApplicationEXI: Serial.println("ApplicationEXI"); break;
			case Thing::CoAP::ContentFormat::ApplicationJSon: Serial.println("ApplicationJSon"); break;
			case Thing::CoAP::ContentFormat::ApplicationLinkFormat: Serial.println("ApplicationLinkFormat"); break;
			case Thing::CoAP::ContentFormat::ApplicationOctetStream: Serial.println("ApplicationOctetStream"); break;
			case Thing::CoAP::ContentFormat::ApplicationXML: Serial.println("ApplicationXML"); break;
			case Thing::CoAP::ContentFormat::TextPlain: Serial.println("TextPlain"); break;
			}
			
			//If the link has title, print the title
			if (link.HasTitle())
			{
				Serial.print("\t");
				Serial.print("Title: ");
				Serial.println(link.GetTitle().c_str());
			}
			
			//If the link has Interface Description, print the Interface Description
			if (link.HasInterfaceDescription())
			{
				Serial.print("\t");
				Serial.print("Interface Description: ");
				Serial.println(link.GetInterfaceDescription().c_str());
			}
			
			//If the link has Maximum Size Estimate, print the Maximum Size Estimate
			if (link.HasMaximumSizeEstimate())
			{
				Serial.print("\t");
				Serial.print("Maximum Size Estimate: ");
				Serial.println(link.GetMaximumSizeEstimate());
			}
			
			//If the link has Resource Type, print the Resource Type
			if (link.HasResourceType())
			{
				Serial.print("\t");
				Serial.print("Resource Type: ");
				Serial.println(link.GetResourceType().c_str());
			}
		}
	});
}

void loop() {
  coapClient.Process();
}
