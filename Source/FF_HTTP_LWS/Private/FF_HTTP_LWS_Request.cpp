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

		// LWS Callback can trigger functions multiple times. So, we need to check that if function could actually catched a header.
		if (EachHeader_Value_Lenght > 0)
		{
			// Key

			const char* EachHeader_Key = (const char*)lws_token_to_string((lws_token_indexes)Index_Header);
			FString EachHeader_Key_String;
			EachHeader_Key_String.AppendChars(EachHeader_Key, strlen(EachHeader_Key));
			
			// Value

			// Add null termination.
			char* EachHeader_Value = (char*)malloc((size_t)EachHeader_Value_Lenght + 1);

			// Add null termination.
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

bool ULwsObject::GetAllUrlParameters(TMap<FString, FString>& Out_Params)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP && this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}

	int Argument_Lenght = 0;
		
	Argument_Lenght = lws_hdr_total_length(this->Params.wsi, lws_token_indexes::WSI_TOKEN_HTTP_URI_ARGS);

	// Add null termination.
	char* Argument_Value = (char*)malloc((size_t)Argument_Lenght + 1);

	Argument_Lenght = lws_hdr_copy(this->Params.wsi, Argument_Value, Argument_Lenght + 1, lws_token_indexes::WSI_TOKEN_HTTP_URI_ARGS);

	FString ArgumentString;
	ArgumentString.AppendChars(Argument_Value, Argument_Lenght);

	TArray<FString> ArgumentPairs;
	ArgumentString.ParseIntoArray(ArgumentPairs, *FString("&"));

	TMap<FString, FString> Temp_Params;
	for (FString EachPair : ArgumentPairs)
	{
		TArray<FString> Sections;
		EachPair.ParseIntoArray(Sections, *FString("="));

		if (Sections.Num() == 2)
		{
			Temp_Params.Add(Sections[0], Sections[1]);
		}
	}

	Out_Params = Temp_Params;

	return true;
}

bool ULwsObject::GetUrlParam(FString& Value, FString Key, int32 BufferSize)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP && this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}

	if (BufferSize <= 0)
	{
		BufferSize = 1024;
	}

	char* Argument = (char*)malloc(BufferSize);

	int Argument_Lenght = 0;
	Argument_Lenght = lws_get_urlarg_by_name_safe(this->Params.wsi, (const char*)TCHAR_TO_UTF8(*(Key + "=")), Argument, BufferSize);

	FString ArgumentString;
	ArgumentString.AppendChars(Argument, Argument_Lenght);

	Value = ArgumentString;

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