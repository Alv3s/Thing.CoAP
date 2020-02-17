#pragma once

#include "IPacketProvider.h"
#include "ObserveToken.h"
#include "ResponseCallback.h"

namespace Thing
{
	namespace CoAP
	{

		class IClient
		{
		public:
			virtual void SetPort(int port) = 0;
			virtual int GetPort() = 0;

			virtual void SetPacketProvider(IPacketProvider* packetProvider) = 0;
			virtual void SetPacketProvider(IPacketProvider& packetProvider) = 0;

			virtual void Discover(ResponseCallback callback) = 0;

			virtual void Get(std::string endpoint, ResponseCallback callback) = 0;
			virtual void Get(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback) = 0;
			virtual void Get(std::string endpoint, std::string payload, ResponseCallback callback) = 0;

			virtual void Post(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback) = 0;
			virtual void Post(std::string endpoint, std::string payload, ResponseCallback callback) = 0;

			virtual void Put(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback) = 0;
			virtual void Put(std::string endpoint, std::string payload, ResponseCallback callback) = 0;

			virtual void Delete(std::string endpoint, std::vector<uint8_t> payload, ResponseCallback callback) = 0;
			virtual void Delete(std::string endpoint, std::string payload, ResponseCallback callback) = 0;

			virtual ObserveToken Observe(std::string endpoint, ResponseCallback callback) = 0;
			virtual void CancelObserve(ObserveToken token) = 0;

			virtual void Start(IPAddress peerIP, int peerPort = 5683) = 0;
			virtual void Process() = 0;
			virtual void Stop() = 0;
		};
	}
}