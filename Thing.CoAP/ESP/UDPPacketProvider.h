#pragma once

#include "../IPacketProvider.h"
#include <WiFiUdp.h>

namespace Thing
{
	namespace CoAP
	{
		namespace ESP
		{
			class UDPPacketProvider : public Thing::CoAP::IPacketProvider
			{
			public:
				virtual void Start(int port) override 
				{
					Udp.begin(port);
				}
				
				virtual void Stop() override
				{
					Udp.stop();
				}

				virtual void SendPacket(std::vector<uint8_t> buffer, Thing::CoAP::IPAddress ip, int port) override
				{
					Udp.beginPacket(ip, port);
					if(buffer.size() > 0)
						Udp.write(&buffer[0], buffer.size());
					Udp.endPacket();
				}
				
				virtual bool ReadPacket(std::vector<uint8_t>* buffer, Thing::CoAP::IPAddress* address, int* port) override
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
