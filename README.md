# Thing.CoAP

Thing.CoAP is a platform independent library implementing both CoAP Server and Client. It has been tested on ESP, ESP32 and a regular Windows application. Linux was not implemented yet although it could be easily implemented.
CoAP is a protocol that operates similar to a REST application: A server has one or several endpoints and has "Get", "Post", "Put" and "Delete" methods. A client can be call the server to get the data. This is useful for constrained devices, like Microcontrollers, turning them capable of receiving requests without the requirement of a broker (like MQTT).

## Getting Started
See folder "Examples" to know how to use it. This library was tested working on Arduino, Platform IO and regular C++ application on Windows.
