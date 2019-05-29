#pragma once

namespace Thing {
	namespace CoAP {
		enum class MessageType
		{
			Confirmable = 0,
			NonConfirmable = 1,
			Acknowledge = 2,
			Reset = 3,
		};
	}
}