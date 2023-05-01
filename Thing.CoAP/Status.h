#pragma once

#include <string>
#include "ResponseCode.h"

namespace Thing
{
	namespace CoAP
	{
		class Status
		{
		public:
			Status(const Status &t);
			virtual ~Status();

			uint8_t *GetPayload() const;
			size_t GetPayloadLength() const;
			Thing::CoAP::ResponseCode GetCode() const;

			static Thing::CoAP::Status Content(uint8_t *data, size_t data_length);
			static Thing::CoAP::Status Content();
			static Thing::CoAP::Status BadRequest();
			static Thing::CoAP::Status MethodNotAllowed();
			static Thing::CoAP::Status InternalServerError();
			static Thing::CoAP::Status PreconditionFailed();
			static Thing::CoAP::Status RequestEntityTooLarge();
			static Thing::CoAP::Status Created(uint8_t *data, size_t data_length);
			static Thing::CoAP::Status Deleted();
			static Thing::CoAP::Status Valid();
			static Thing::CoAP::Status NotFound();
			static Thing::CoAP::Status NotImplemented();

			Status &operator=(const Status &other)
			{
				if (&other == this)
					return *this;

				this->data = other.data;
				this->data_length = other.data_length;
				this->code = other.code;
				return *this;
			}

			bool operator==(const Status &other) const
			{
				return this->code == other.code && this->data == other.data && this->data_length == other.data_length;
			}

			bool operator!=(const Status &other) const
			{
				return !(*this == other);
			}

		private:
			Status(uint8_t *data, size_t data_length, Thing::CoAP::ResponseCode code);
			Status(Thing::CoAP::ResponseCode code);

			uint8_t *data;
			size_t data_length;
			Thing::CoAP::ResponseCode code;
		};
	}
}