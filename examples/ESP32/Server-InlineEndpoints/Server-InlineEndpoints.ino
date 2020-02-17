#include <WiFi.h>

//Include Thing.CoAP
#include "Thing.CoAP.h"

//[RECOMMENDED] Alternatively, if you are NOT using Arduino IDE you can include each file you need as bellow: 
//#include "Thing.CoAP/Server.h"
//#include "Thing.CoAP/ESP/UDPPacketProvider.h"

//Declare our CoAP server and the packet handler
Thing::CoAP::Server server;
Thing::CoAP::ESP::UDPPacketProvider udpProvider;

//Change here your WiFi settings
char* ssid = "YourWiFiSSID";
char* password = "YourWiFiPassword";

#define LED 2
#define BUTTON 0

void setup() {
  //Initializing the Serial
  Serial.begin(115200);
  Serial.println("Initializing");

  //Configure the LED as output
  pinMode(LED, OUTPUT);
  //Configure Button
  pinMode(BUTTON, INPUT_PULLUP);
  
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
  server.SetPacketProvider(udpProvider);

  //Create an resource called "LED"
  server.CreateResource("LED", Thing::CoAP::ContentFormat::TextPlain, true) //True means that this resource is observable
    .OnGet([](Thing::CoAP::Request & request) { //We are here configuring telling our server that, when we receive a "GET" request to this endpoint, run the the following code
      Serial.println("GET Request received for endpoint 'LED'");

      //Read the state of our led.
      std::string result;
      if(digitalRead(LED) == HIGH)
        result = "On";
      else
        result = "Off";

       //Return the current state of our "LED".
      return Thing::CoAP::Status::Content(result);
    }).OnPost([](Thing::CoAP::Request& request) {  //We are here configuring telling our server that, when we receive a "POST" request to this endpoint, run the the following code
      Serial.println("POST Request received for endpoint 'LED'");

      //Get the message sent fromthe client and parse it to a string      
      auto payload = request.GetPayload();
      std::string message(payload.begin(), payload.end());
      
      Serial.print("The client sent the message: ");
      Serial.println(message.c_str());

      if(message == "On") { //If the message is "On" we will turn the LED on.
        digitalWrite(LED, HIGH);
      } else if (message == "Off") { //If it is "Off" we will turn the LED off.
        digitalWrite(LED, LOW);
      } else { //In case any other message is received we will respond a "BadRequest" error.
        return Thing::CoAP::Status::BadRequest();
      }

      //In case "On" or "Off" was received, we will return "Ok" with a message saying "Command Executed".
      return Thing::CoAP::Status::Content("Command Executed");
    });

  //Create an resource called "Button"
  server.CreateResource("Button", Thing::CoAP::ContentFormat::TextPlain, false)
    .OnGet([](Thing::CoAP::Request & request) { //We are here configuring telling our server that, when we receive a "GET" request to this endpoint, run the the following code
      Serial.println("GET Request received for endpoint 'Button'");

      //Read the state of our led.
      std::string result;
      if(digitalRead(BUTTON) == HIGH)
        result = "On";
      else
        result = "Off";

       //Return the current state of our "LED".
      return Thing::CoAP::Status::Content(result);
    });

    server.Start();
}

void loop() {
  server.Process();
}
