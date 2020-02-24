#include "Status.h"

namespace Thing {
	namespace CoAP
	{

		Status::Status(std::string payload, Thing::CoAP::ResponseCode code) : payload(payload), code(code)
		{
		}

		Status::Status(Thing::CoAP::ResponseCode code) : payload(""), code(code)
		{

		}

		Status::Status(const Status &t) : payload(t.payload), code(t.code)
		{
			
		}

		Status::~Status()
		{
		}

		std::string Status::GetPayload() const
		{
			return payload;
		}

		Thing::CoAP::ResponseCode Status::GetCode() const
		{
			return code;
		}

		Status Status::Content(std::string payload)
		{
			return Status(payload, Thing::CoAP::ResponseCode::Content);
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

		Status Status::Created(std::string payload)
		{
			return Status(payload, Thing::CoAP::ResponseCode::Created);
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