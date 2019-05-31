#include "ObserveToken.h"

namespace Thing
{
	namespace CoAP
	{
		ObserveToken::ObserveToken()
		{
		}
		ObserveToken::ObserveToken(uint16_t token, std::string endpoint, ResponseCallback callback) :
			token(token),
			endpoint(endpoint),
			callback(callback)
		{
		}

		ObserveToken::~ObserveToken()
		{
		}

		uint16_t ObserveToken::GetToken()
		{
			return token;
		}

		std::string ObserveToken::GetEndpoint()
		{
			return endpoint;
		}

		ResponseCallback ObserveToken::GetCallback()
		{
			return callback;
		}
	}
}