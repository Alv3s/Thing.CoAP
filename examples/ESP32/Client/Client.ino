#include <WiFi.h>

//Include Thing.CoAP
#include "Thing.CoAP.h"

//[RECOMMENDED] Alternatively, if you are NOT using Arduino IDE you can include each file you need as bellow: 
//#include "Thing.CoAP/Client.h"
//#include "Thing.CoAP/ESP/UDPPacketProvider.h"

//Declare our CoAP client and the packet handler
Thing::CoAP::Client coapClient;
Thing::CoAP::ESP::UDPPacketProvider udpProvider;

//Change here your WiFi settings
char* ssid = "YourWiFiSSID";
char* password = "YourWiFiPassword";

void sendMessage(){
  //Make a post
  coapClient.Get("create1", "", [](Thing::CoAP::Response response){
      std::vector<uint8_t> payload = response.GetPayload();
      std::string received(payload.begin(), payload.end());
      Serial.println("Server sent the following message:");
      Serial.println(received.c_str());
      delay(5000);
      sendMessage();
  });
}

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
  
  //Send A Message
  sendMessage();
}

void loop() {
  coapClient.Process();
}
