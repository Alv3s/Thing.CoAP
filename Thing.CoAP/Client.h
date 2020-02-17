#pragma once

#include "IClient.h"
#include "Response.h"
#include <vector>
#include <map>
#include <functional>
#include "Status.h"
#include "IPAddress.h"
#include "Packet.h"

namespace Thing
{
	namespace CoAP
	{
		class Client : public virtual IClient
		{
		public:
			Client();
			~Client();

			void SetPort(int port) override;
			int GetPort() override;

			void SetPacketProvider(IPacketProvider* packetProvider) override;
			void SetPacketProvider(IPacketProvider& packetProvider) override;

			void Discover(ResponseCallback callback) override;

			void Get(std::string endpoint, ResponseCallback callback) override;
			void Get(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback) override;
			void Get(std::string endpoint, std::string payload, ResponseCallback callback) override;

			void Post(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback) override;
			void Post(std::string endpoint, std::string payload, ResponseCallback callback) override;

			void Put(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback) override;
			void Put(std::string endpoint, std::string payload, ResponseCallback callback) override;

			void Delete(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback) override;
			void Delete(std::string endpoint, std::string payload, ResponseCallback callback) override;

			ObserveToken Observe(std::string endpoint, ResponseCallback callback) override;
			void CancelObserve(ObserveToken token) override;

			void Start(IPAddress peerIP, int peerPort = 5683) override;
			void Process() override;
			void Stop() override;
		private:
			IPacketProvider* packetProvider;
			std::map<uint16_t, ResponseCallback> pendingRequests;
			std::map<uint16_t, ObserveToken> observeRequests;
			uint16_t currentMessageId;

			IPAddress peerIP;
			int peerPort;
			int port;

			void Request(std::string endpoint, Method method, std::vector<uint8_t> payload, ResponseCallback callback);
			void Request(std::string endpoint, Method method, std::string payload, ResponseCallback callback);
		};
	}
}

