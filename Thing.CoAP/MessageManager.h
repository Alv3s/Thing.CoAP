#pragma once

#include <map>
#include <vector>
#include "Packet.h"

namespace Thing {
	namespace CoAP
	{
		class MessageManager
		{
		public:
			MessageManager();
			~MessageManager();


		private:
			std::map<unsigned long, std::vector<Thing::CoAP::Packet> > queue;
		};
	}
}