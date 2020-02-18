# Thing.CoAP

Thing.CoAP is a platform independent library implementing both CoAP Server and Client. It has been tested on ESP, ESP32 and a regular Windows application. Linux was not implemented yet although it could be easily implemented.

## Features
CoAP is a protocol that operates similar to a REST application: A server has one or several endpoints (caleed Resources) accessible via "Get", "Post", "Put" and "Delete" methods. A client can be call the server to get the data. This is useful for constrained devices, like Microcontrollers, allowing the communication between these devices or a computer. One of the advantages of CoAp when comparing to MQTT is that CoAP protocol doesn't require a broker, making an easier to deploy environment without a single failure point.

The following architectures/devices were tested and have examples for:
- Windows
- ESP8266
- ESP32

### Server Features
- **Any number of resources**:  Thing.CoAP allows to add any number of Resources.
	- Get: Get method is implemented. Typically used to read the state of a resource.
	- Post: Post method is implemented. Typically used to toggle the state of a resource or create a resource.
	- Put: Put method is implemented. Typically used to update the state of a resource.
	- Delete: Delete Method is implemented. Typically used to disable or delete a resource.
- **Observe**: All resources can be observed by any number of clients, allowing the clients to receive notifications when the resource has changed it's state.
- **Discovery**: Discovery is implemented, allowing a client to discover automatically all available resources and their properties.
- **Multicast Discovery**: Allows the discovery of all CoAP devices in the network using multicast.

## Client Features
- **Get**: Get method is implemented. Typically used to read the state of a resource.
- **Post**: Post method is implemented. Typically used to toggle the state of a resource or create a resource.
- **Put**: Put method is implemented. Typically used to update the state of a resource.
- **Delete**: Delete Method is implemented. Typically used to disable or delete a resource.
- **Discover**: Allows the discovery of the resources of a client.
- **Observe**: Allows the observation of a resource.

## Getting Started
See folder "Examples" to know how to use it. This library was tested working on Arduino, Platform IO and regular C++ application on Windows.

## Current State and Mantainability
The Repository has unit tests that cover virtually 100% of the code, allowing the code to be regression free and virtually bug free. The code was developed with portability in mind, allowing it to be easily implemented on any compiler or device that supports C++11 std functions.
