#include "Status.h"

namespace Thing
{
	namespace CoAP
	{

		Status::Status(uint8_t *data, size_t data_length, Thing::CoAP::ResponseCode code) : data(data), data_length(data_length), code(code) {}

		/*
		Status::Status(std::string payload, Thing::CoAP::ResponseCode code) : payload(payload), code(code)
		{
		}
		*/

		Status::Status(Thing::CoAP::ResponseCode code) : data(NULL), data_length(0), code(code)
		{
		}

		Status::Status(const Status &t) : data(t.data), data_length(t.data_length), code(t.code)
		{
		}

		Status::~Status()
		{
		}

		uint8_t *Status::GetPayload() const
		{
			return data;
		}

		size_t Status::GetPayloadLength() const
		{
			return data_length;
		}

		Thing::CoAP::ResponseCode Status::GetCode() const
		{
			return code;
		}

		Status Status::Content(uint8_t *data, size_t data_length)
		{
			return Status(data, data_length, Thing::CoAP::ResponseCode::Content);
		}

		Thing::CoAP::Status Status::Content()
		{
			return Status(Thing::CoAP::ResponseCode::Content);
		}

		Thing::CoAP::Status Status::BadRequest()
		{
			return Status(Thing::CoAP::ResponseCode::BadRequest);
		}

		Thing::CoAP::Status Status::MethodNotAllowed()
		{
			return Status(Thing::CoAP::ResponseCode::MethodNotAllowed);
		}

		Thing::CoAP::Status Status::InternalServerError()
		{
			return Status(Thing::CoAP::ResponseCode::InternalServerError);
		}

		Thing::CoAP::Status Status::PreconditionFailed()
		{
			return Status(Thing::CoAP::ResponseCode::PreconditionFailed);
		}

		Thing::CoAP::Status Status::RequestEntityTooLarge()
		{
			return Status(Thing::CoAP::ResponseCode::RequestEntityTooLarge);
		}

		Status Status::Created(uint8_t *data, size_t data_length)
		{
			return Status(data, data_length, Thing::CoAP::ResponseCode::Created);
		}

		Status Status::Deleted()
		{
			return Status(Thing::CoAP::ResponseCode::Deleted);
		}

		Status Status::Valid()
		{
			return Status(Thing::CoAP::ResponseCode::Valid);
		}

		Status Status::NotFound()
		{
			return Status(Thing::CoAP::ResponseCode::NotFound);
		}

		Status Status::NotImplemented()
		{
			return Status(Thing::CoAP::ResponseCode::NotImplemented);
		}
	}
}