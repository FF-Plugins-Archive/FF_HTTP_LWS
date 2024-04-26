#include "FF_HTTP_LWS_Request.h"

bool UHttpConnectionLws::GetUri(FString& Out_Uri)
{
	if (!this->CallbackParams.in)
	{
		return false;
	}

	FString TempUri = (const char*)this->CallbackParams.in;
	Out_Uri = TempUri;

	return true;
}

bool UHttpConnectionLws::GetBody(FString& Out_Body)
{
	if (!this->CallbackParams.wsi)
	{
		return false;
	}

	return true;
}

bool UHttpConnectionLws::GetHeader(TMap<FString, FString>& Out_Headers)
{
	if (!this->CallbackParams.wsi)
	{
		return false;
	}

	return true;
}

bool UHttpConnectionLws::GetParams(TMap<FString, FString>& Out_Params)
{
	if (!this->CallbackParams.wsi)
	{
		return false;
	}

	return true;
}

bool UHttpConnectionLws::SendResponse(const FString In_Response, TMap<FString, FString> In_Header, const bool bAddAllowOrigin, int32 Status_Code)
{
#ifdef _WIN64

	if (!this->CallbackParams.wsi)
	{
		return false;
	}

	return true;

#else
	return false;
#endif
}