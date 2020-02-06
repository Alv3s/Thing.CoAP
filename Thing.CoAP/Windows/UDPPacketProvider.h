#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "../IPacketProvider.h"

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

namespace Thing {
	namespace CoAP {
		namespace Windows {

			template <int BUFLEN>
			class UDPPacketProvider : public virtual Thing::CoAP::IPacketProvider
			{
			public:
				void Start(int port) override
				{
					this->port = port;

					WSADATA wsaData;
					if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
					{
						std::cout << "Error!" << std::endl;
						return;
					}

					//Create a socket
					if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
						std::cout << "Error!" << std::endl;

					//Prepare the sockaddr_in structure
					local.sin_family = AF_INET;
					local.sin_addr.s_addr = INADDR_ANY;
					local.sin_port = htons(this->port);

					//Bind
					if (bind(s, (struct sockaddr*) & local, sizeof(local)) == SOCKET_ERROR)
					{
						std::cout << "Error!" << std::endl;
						closesocket(s);
					}
				}

				void Stop() override
				{
					closesocket(s);
					WSACleanup();
				}

				void SendPacket(std::vector<uint8_t> buffer, Thing::CoAP::IPAddress ip, int port) override
				{
					if (buffer.size() == 0)
						return;

					struct sockaddr_in serverInfo;
					serverInfo.sin_family = AF_INET;
					serverInfo.sin_port = htons(port);
					serverInfo.sin_addr.s_addr = ip;
					if (sendto(s, (char*)&buffer[0], buffer.size(), 0, (sockaddr*)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR)
					{
						printf("sendto() failed with error code : %d", WSAGetLastError());
						return;
					}
				}

				bool ReadPacket(std::vector<uint8_t>* buffer, Thing::CoAP::IPAddress* address, int* port) override
				{
					//try to receive some data, this is a blocking call
					sockaddr_in si_other;
					int slen = sizeof(si_other);
					int recv_len;
					if ((recv_len = recvfrom(s, (char*)buf, BUFLEN, 0, (struct sockaddr*) & si_other, &slen)) == SOCKET_ERROR)
					{
						std::cout << "Error!" << std::endl;
						closesocket(s);
						return false;
					}

					*buffer = std::vector<uint8_t>(buf, buf + recv_len);
					*address = static_cast<uint32_t>(si_other.sin_addr.s_addr);
					*port = ntohs(si_other.sin_port);

					return true;
				}

			private:
				uint8_t buf[BUFLEN];
				int port;

				SOCKET s;
				sockaddr_in local;
			};
		}
	}
}