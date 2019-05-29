#include "StatusTest.h"
#include "../Thing.CoAP/Status.h"

namespace Thing {
	namespace CoAP {
		namespace Tests {
			void StatusTest::SetUp()
			{

			}

			void StatusTest::TearDown()
			{

			}

			TEST_F(StatusTest, OperatorsTest)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::Ok();
				Thing::CoAP::Status status1 = Thing::CoAP::Status::Ok();
				EXPECT_EQ(status, status1);

				Thing::CoAP::Status status2 = Thing::CoAP::Status::Ok("test");
				EXPECT_NE(status, status2);

				Thing::CoAP::Status status3 = Thing::CoAP::Status::BadRequest();
				EXPECT_NE(status, status3);
			}

			TEST_F(StatusTest, OKStatusWithoutPayload)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::Ok();

				EXPECT_EQ(0, status.GetPayload().size());
				EXPECT_EQ(Thing::CoAP::ResponseCode::Ok, status.GetCode());
			}

			TEST_F(StatusTest, OKStatusWithPayload)
			{
				const std::string payload = "ABCDEFG";
				Thing::CoAP::Status status = Thing::CoAP::Status::Ok(payload);

				EXPECT_EQ(payload, status.GetPayload());
				EXPECT_EQ(Thing::CoAP::ResponseCode::Ok, status.GetCode());
			}

			TEST_F(StatusTest, CreatedStatus)
			{
				const std::string payload = "ABCDEFG";
				Thing::CoAP::Status status = Thing::CoAP::Status::Created(payload);

				EXPECT_EQ(payload, status.GetPayload());
				EXPECT_EQ(Thing::CoAP::ResponseCode::Created, status.GetCode());
			}

			TEST_F(StatusTest, DeletedStatus)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::Deleted();
				EXPECT_EQ(Thing::CoAP::ResponseCode::Deleted, status.GetCode());
			}

			TEST_F(StatusTest, ValidStatus)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::Valid();
				EXPECT_EQ(Thing::CoAP::ResponseCode::Valid, status.GetCode());
			}

			TEST_F(StatusTest, NotFoundStatus)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::NotFound();
				EXPECT_EQ(Thing::CoAP::ResponseCode::NotFound, status.GetCode());
			}

			TEST_F(StatusTest, NotImplementedStatus)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::NotImplemented();
				EXPECT_EQ(Thing::CoAP::ResponseCode::NotImplemented, status.GetCode());
			}

			TEST_F(StatusTest, BadRequestStatus)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::BadRequest();
				EXPECT_EQ(Thing::CoAP::ResponseCode::BadRequest, status.GetCode());
			}

			TEST_F(StatusTest, MethodNotAllowedStatus)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::MethodNotAllowed();
				EXPECT_EQ(Thing::CoAP::ResponseCode::MethodNotAllowed, status.GetCode());
			}

			TEST_F(StatusTest, InternalServerErrorStatus)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::InternalServerError();
				EXPECT_EQ(Thing::CoAP::ResponseCode::InternalServerError, status.GetCode());
			}

			TEST_F(StatusTest, PreconditionFailedStatus)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::PreconditionFailed();
				EXPECT_EQ(Thing::CoAP::ResponseCode::PreconditionFailed, status.GetCode());
			}

			TEST_F(StatusTest, RequestEntityTooLargeStatus)
			{
				Thing::CoAP::Status status = Thing::CoAP::Status::RequestEntityTooLarge();
				EXPECT_EQ(Thing::CoAP::ResponseCode::RequestEntityTooLarge, status.GetCode());
			}
		}
	}
}