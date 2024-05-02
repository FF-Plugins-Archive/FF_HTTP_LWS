#include "FF_HTTP_LWS_Request.h"

bool ULwsObject::GetCustomHeader(FString& Value, FString Key)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP && this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}

	const char* HeaderKey = TCHAR_TO_UTF8(*(Key + ":"));
	size_t HeaderKeyLenght = strlen(HeaderKey);
	size_t HeaderValueLenght = 0;

	HeaderValueLenght = lws_hdr_custom_length(this->Params.wsi, HeaderKey, strlen(HeaderKey));

	// Give a better allocation value.
	char* custom_header_value = (char*)malloc(256);

	HeaderValueLenght = lws_hdr_custom_copy(this->Params.wsi, custom_header_value, 256, HeaderKey, HeaderKeyLenght);

	FString HeaderString;
	HeaderString.AppendChars(custom_header_value, HeaderValueLenght);

	Value = HeaderString;

	return true;
}

bool ULwsRequest::GetUri(FString& Out_Uri)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP || !this->Params.in)
	{
		return false;
	}

	char* UriPtr = NULL;
	int UriLenght = 0;
	int GetResult = lws_http_get_uri_and_method(this->Params.wsi, &UriPtr, &UriLenght);

	FString TempString;
	TempString.AppendChars(UriPtr, UriLenght);

	Out_Uri = TempString;
	return true;
}

bool ULwsRequest::SendResponse(const FString In_Response, TMap<FString, FString> In_Header, const bool bAddAllowOrigin, int32 Status_Code)
{
	if (!this->Params.wsi)
	{
		return false;
	}

	this->RetVal = lws_callback_http_dummy(this->Params.wsi, this->Params.reason, this->Params.user, this->Params.in, this->Params.len);
	return true;
}

bool ULwsInfos::GetBody(FString& Out_Body)
{
	if (this->Params.reason == LWS_CALLBACK_HTTP_BODY && this->Params.in)
	{
		const char* Body = (const char*)this->Params.in;
		const size_t BodyLenght = this->Params.len;

		FString BodyString;
		BodyString.AppendChars(Body, BodyLenght);

		Out_Body = BodyString;

		return true;
	}

	else
	{
		return false;
	}
}