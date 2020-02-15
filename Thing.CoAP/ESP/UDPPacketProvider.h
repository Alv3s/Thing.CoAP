#pragma once

#include "../IPacketProvider.h"
#include <WiFiUdp.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

namespace Thing
{
	namespace CoAP
	{
		namespace ESP
		{
			class UDPPacketProvider : public Thing::CoAP::IPacketProvider
			{
			public:
				void Start(int port) override 
				{
					::IPAddress multicastAddress(224, 0, 1, 187);
#ifdef ESP8266
					Udp.beginMulticast(WiFi.localIP(), multicastAddress, port);
#else
					Udp.beginMulticast(multicastAddress, port);
#endif
					Udp.begin(port);
				}
				
				void Stop() override
				{
					Udp.stop();
				}

				void SendPacket(std::vector<uint8_t> buffer, Thing::CoAP::IPAddress ip, int port) override
				{
					if(ip & 0xE0000000) //If is multicast
#ifdef ESP8266
						Udp.beginPacketMulticast(ip, port, WiFi.localIP());
#else
						Udp.beginMulticastPacket();
#endif
					else
						Udp.beginPacket(ip, port);
						
					if(buffer.size() > 0)
						Udp.write(&buffer[0], buffer.size());
					Udp.endPacket();
				}
				
				bool ReadPacket(std::vector<uint8_t>* buffer, Thing::CoAP::IPAddress* address, int* port) override
				{
					int32_t packetlen = Udp.parsePacket();
					*buffer = std::vector<uint8_t>(packetlen);
					int length = Udp.read(&(*buffer)[0], packetlen);
					*address = Udp.remoteIP();
					*port = Udp.remotePort();
					return length > 0;
				}
			private:
				WiFiUDP Udp;
			};
		}
	}
}
