#pragma once

#include <string>
#include "ResponseCode.h"

namespace Thing {
	namespace CoAP
	{
		class Status
		{
		public:
			Status(const Status &t);
			virtual ~Status();

			std::string GetPayload() const;
			Thing::CoAP::ResponseCode GetCode() const;

			static Thing::CoAP::Status Content(std::string payload);
			static Thing::CoAP::Status Content();
			static Thing::CoAP::Status BadRequest();
			static Thing::CoAP::Status MethodNotAllowed();
			static Thing::CoAP::Status InternalServerError();
			static Thing::CoAP::Status PreconditionFailed();
			static Thing::CoAP::Status RequestEntityTooLarge();
			static Thing::CoAP::Status Created(std::string payload);
			static Thing::CoAP::Status Deleted();
			static Thing::CoAP::Status Valid();
			static Thing::CoAP::Status NotFound();
			static Thing::CoAP::Status NotImplemented();

			bool operator==(const Status& other) const
			{
				return this->code == other.code && this->payload == other.payload;
			}

			bool operator!=(const Status& other) const
			{
				return !(*this == other);
			}
		private:
			Status(std::string payload, Thing::CoAP::ResponseCode code);
			Status(Thing::CoAP::ResponseCode code);

			const std::string payload;
			const Thing::CoAP::ResponseCode code;
		};
	}
}