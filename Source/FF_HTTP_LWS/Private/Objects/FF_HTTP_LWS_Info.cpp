#include "Objects/FF_HTTP_LWS_Info.h"

bool ULwsInfos::GetBody(FString& Out_Body)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}

	if (!this->Params.in)
	{
		return false;
	}

	const char* Body = (const char*)this->Params.in;
	const size_t BodyLenght = this->Params.len;

	FString BodyString;
	BodyString.AppendChars(Body, BodyLenght);

	Out_Body = BodyString;

	return true;
}