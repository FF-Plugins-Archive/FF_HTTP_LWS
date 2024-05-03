#include "FF_HTTP_LWS_Request.h"

bool ULwsObject::GetCustomHeader(FString& Value, FString Key)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP && this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}

	const char* HeaderKey = TCHAR_TO_UTF8(*(Key + ":"));
	size_t HeaderKey_Lenght = strlen(HeaderKey);
	
	int HeaderValue_Lenght = 0;

	// Add null termination.
	HeaderValue_Lenght = lws_hdr_custom_length(this->Params.wsi, HeaderKey, strlen(HeaderKey)) + 1;

	char* HeaderValue = (char*)malloc(HeaderValue_Lenght);

	HeaderValue_Lenght = (size_t)lws_hdr_custom_copy(this->Params.wsi, HeaderValue, HeaderValue_Lenght, HeaderKey, HeaderKey_Lenght);

	FString HeaderString;
	HeaderString.AppendChars(HeaderValue, HeaderValue_Lenght);

	Value = HeaderString;

	return true;
}

bool ULwsObject::GetAllCustomHeaders(TArray<FString> In_Headers, TMap<FString, FString>& Out_Headers)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP && this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}

	TMap<FString, FString> Temp_Headers;
	for (FString EachHeader_Key : In_Headers)
	{
		FString EachHeader_Value;
		this->GetCustomHeader(EachHeader_Value, EachHeader_Key);

		Temp_Headers.Add(EachHeader_Key, EachHeader_Value);
	}

	Out_Headers = Temp_Headers;

	return true;
}

bool ULwsObject::GetAllKnownHeaders(TMap<FString, FString>& Out_Headers)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP && this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}

	TMap<FString, FString> Temp_Headers;

	for (int Index_Header = 0; Index_Header < WSI_TOKEN_COUNT; Index_Header++)
	{
		int EachHeader_Value_Lenght = lws_hdr_total_length(this->Params.wsi, (lws_token_indexes)Index_Header);

		if (EachHeader_Value_Lenght > 0)
		{
			// Key

			const char* EachHeader_Key = (const char*)lws_token_to_string((lws_token_indexes)Index_Header);
			FString EachHeader_Key_String;
			EachHeader_Key_String.AppendChars(EachHeader_Key, strlen(EachHeader_Key));
			
			// Value

			char* EachHeader_Value = (char*)malloc((size_t)EachHeader_Value_Lenght + 1);
			EachHeader_Value_Lenght = lws_hdr_copy(this->Params.wsi, EachHeader_Value, EachHeader_Value_Lenght + 1, (lws_token_indexes)Index_Header);

			FString EachHeader_Value_String;
			EachHeader_Value_String.AppendChars(EachHeader_Value, EachHeader_Value_Lenght);

			// Result

			Temp_Headers.Add(EachHeader_Key_String, EachHeader_Value_String);
		}
	}

	Out_Headers = Temp_Headers;

	return true;
}

bool ULwsRequest::GetUri(FString& Out_Uri)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP)
	{
		return false;
	}

	if (!this->Params.in)
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