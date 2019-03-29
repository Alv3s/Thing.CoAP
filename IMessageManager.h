#pragma once

#include "Packet.h"

namespace Thing {
	namespace CoAP
	{
		class IMessageManager
		{
		public:
			virtual void SetAcknowledgeTimeout(unsigned long millis) = 0;
			virtual void SetAcknowledgeRandomFactor(float factor) = 0;
			virtual void SetMaxRetransmit(int retransmitCount) = 0;
			virtual void SetNStart(int nStart) = 0;
			virtual void SetDefaultLeisure(unsigned long millis) = 0;
			virtual void SetProbingRate(int rate) = 0;

			virtual void AddConfirmablePacket(Thing::CoAP::Packet& packet) = 0;
		};
	}
}