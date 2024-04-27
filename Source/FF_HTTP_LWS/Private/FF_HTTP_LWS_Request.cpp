#include "FF_HTTP_LWS_Request.h"

FString UHttpConnectionLws::ParseReceived()
{
	return (const char*)this->CallbackParams.in;
}

bool UHttpConnectionLws::GetUri(FString& Out_Uri)
{
	if (this->CallbackParams.reason == LWS_CALLBACK_HTTP && this->CallbackParams.in)
	{
		char* UriPtr = NULL;
		int UriLenght = 0;
		int GetResult = lws_http_get_uri_and_method(this->CallbackParams.wsi, &UriPtr, &UriLenght);

		FString TempString;
		TempString.AppendChars(UriPtr, UriLenght);

		Out_Uri = TempString;
		return true;
	}
	
	else
	{
		return false;
	}
}

bool UHttpConnectionLws::GetBody(FString& Out_Body)
{
	if (this->CallbackParams.reason == LWS_CALLBACK_HTTP_BODY && this->CallbackParams.in)
	{
		Out_Body = this->ParseReceived();
		return true;
	}

	else
	{
		return false;
	}
}

bool UHttpConnectionLws::GetHeader(FString& Value, FString Key)
{
	const char* custom_header_name = "X-Custom-Header";
	int header_length = lws_hdr_custom_length(this->CallbackParams.wsi, custom_header_name, 16);
	
	char buffer[256];
	int retval = lws_hdr_custom_copy(this->CallbackParams.wsi, buffer, sizeof(buffer), custom_header_name, strlen(custom_header_name));

	UE_LOG(LogTemp, Warning, TEXT("Header Lenght : %d // RetVal : %d"), header_length, retval);

	FString TempString;
	//TempString.AppendChars(buffer, retval);

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