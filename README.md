# Thing.CoAP

![C/C++ CI](https://github.com/Alv3s/Thing.CoAP/workflows/C/C++%20CI/badge.svg?branch=master)

Thing.CoAP is a platform independent library implementing both CoAP Server and Client. It has been tested on ESP, ESP32 and regular Windows/Linux application.

## Features
CoAP is a protocol that operates similar to a REST application: A server has one or several endpoints (called Resources) accessible via "Get", "Post", "Put" and "Delete" methods. A client can be call the server to get the data. This is useful for constrained devices, like Microcontrollers, allowing the communication between these devices or a computer. One of the advantages of CoAp when comparing to MQTT is that CoAP protocol doesn't require a broker, making an easier to deploy environment without a single failure point.

The following architectures/devices were tested and have examples for:
- Windows
- ESP8266
- ESP32
- Linux

### Server Features
- **Any number of resources**:  Thing.CoAP allows to add any number of Resources.
	- Get: Get method is implemented. Typically used to read the state of a resource.
	- Post: Post method is implemented. Typically used to toggle the state of a resource or create a resource.
	- Put: Put method is implemented. Typically used to update the state of a resource.
	- Delete: Delete Method is implemented. Typically used to disable or delete a resource.
- **Observe**: All resources can be observed by any number of clients, allowing the clients to receive notifications when the resource has changed it's state.
- **Discovery**: Discovery is implemented, allowing a client to discover automatically all available resources and their properties.
- **Multicast Discovery**: Allows the discovery of all CoAP devices in the network using multicast.

### Client Features
- **Get**: Get method is implemented. Typically used to read the state of a resource.
- **Post**: Post method is implemented. Typically used to toggle the state of a resource or create a resource.
- **Put**: Put method is implemented. Typically used to update the state of a resource.
- **Delete**: Delete Method is implemented. Typically used to disable or delete a resource.
- **Discover**: Allows the discovery of the resources of a client.
- **Observe**: Allows the observation of a resource.

## Getting Started - Examples
See folder ```examples``` to know how to use it. This library was tested working on Arduino, Platform IO and regular C++ application on Windows and Linux.

### ESP32 and ESP8266 via Arduino
To use this library via arduino, copy this repository to your arduino libraries folder. After this step, restart Arduino IDE and you will find the examples listed on ```File -> Examples -> Thing.CoAP```.

### Windows
Copy this repository. On it's root folder open the solution file ```Thing.CoAP.sln```. This file will contain actual Thing.CoAP project alongside examples and unit tests.

### Linux
Copy this repository. Inside it:
```
$ autoreconf -i
$ ./configure
$ make
$ make install
```
You will find the compiled examples on folder ```examples```.

## Regression Tests
The Repository has unit tests that cover virtually 100% of the code, allowing the code to have the least ammount of regressions throught the implementation of new features and bug fixing. Let's keep it that way.
