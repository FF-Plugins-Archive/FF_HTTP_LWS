#include "Objects/FF_HTTP_LWS_Object.h"

ELwsKnownHeaders ULwsObject::ConvertBpHeader(lws_token_indexes HeaderType)
{
	return ELwsKnownHeaders();
}

lws_token_indexes ULwsObject::ConvertLwsHeader(ELwsKnownHeaders HeaderType)
{
	return lws_token_indexes();
}

bool ULwsObject::GetMethod(FString& Out_Method)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP)
	{
		return false;
	}

	if (lws_hdr_total_length(this->Params.wsi, WSI_TOKEN_GET_URI) > 0)
	{
		Out_Method = "GET";
		return true;
	}

	else if (lws_hdr_total_length(this->Params.wsi, WSI_TOKEN_POST_URI) > 0)
	{
		Out_Method = "POST";
		return true;
	}

	else if (lws_hdr_total_length(this->Params.wsi, WSI_TOKEN_PUT_URI) > 0)
	{
		Out_Method = "PUT";
		return true;
	}

	else if (lws_hdr_total_length(this->Params.wsi, WSI_TOKEN_PATCH_URI) > 0)
	{
		Out_Method = "PATCH";
		return true;
	}

	else if (lws_hdr_total_length(this->Params.wsi, WSI_TOKEN_DELETE_URI) > 0)
	{
		Out_Method = "DELETE";
		return true;
	}

	else if (lws_hdr_total_length(this->Params.wsi, WSI_TOKEN_HEAD_URI) > 0)
	{
		Out_Method = "HEAD";
		return true;
	}

	else if (lws_hdr_total_length(this->Params.wsi, WSI_TOKEN_OPTIONS_URI) > 0)
	{
		Out_Method = "OPTIONS";
		return true;
	}

	return false;
}

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

bool ULwsObject::GetKnownHeader_Enum(FString& Value, ELwsKnownHeaders Key)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP && this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}
	
	lws_token_indexes HeaderToken = this->ConvertLwsHeader(Key);

	int Header_Value_Lenght = lws_hdr_total_length(this->Params.wsi, HeaderToken);

	if (Header_Value_Lenght > 0)
	{
		// Add null termination.
		char* Header_Value = (char*)malloc((size_t)Header_Value_Lenght + 1);

		// Add null termination.
		Header_Value_Lenght = lws_hdr_copy(this->Params.wsi, Header_Value, Header_Value_Lenght + 1, HeaderToken);

		FString TempValue;
		TempValue.AppendChars(Header_Value, Header_Value_Lenght);
		Value = TempValue;

		return true;
	}

	else
	{
		return false;
	}
}

bool ULwsObject::GetKnownHeader_Int(FString& Value, int32 Key)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP && this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}

	int Header_Value_Lenght = lws_hdr_total_length(this->Params.wsi, (lws_token_indexes)Key);

	if (Header_Value_Lenght > 0)
	{
		// Add null termination.
		char* Header_Value = (char*)malloc((size_t)Header_Value_Lenght + 1);

		// Add null termination.
		Header_Value_Lenght = lws_hdr_copy(this->Params.wsi, Header_Value, Header_Value_Lenght + 1, (lws_token_indexes)Key);

		FString TempValue;
		TempValue.AppendChars(Header_Value, Header_Value_Lenght);
		Value = TempValue;

		return true;
	}

	else
	{
		return false;
	}
}

bool ULwsObject::GetAllKnownHeaders(TMap<FString, FLwsKnownHeaders>& Out_Headers)
{
	if (this->Params.reason != LWS_CALLBACK_HTTP && this->Params.reason != LWS_CALLBACK_HTTP_BODY)
	{
		return false;
	}

	TMap<FString, FLwsKnownHeaders> Temp_Headers;

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

			FLwsKnownHeaders EachHeaderValue;
			EachHeaderValue.String.AppendChars(EachHeader_Value, EachHeader_Value_Lenght);
			
			// Extra Data
			EachHeaderValue.Index = Index_Header;
			EachHeaderValue.HeaderEnum = this->ConvertBpHeader((lws_token_indexes)Index_Header);

			// Result

			Temp_Headers.Add(EachHeader_Key_String, EachHeaderValue);
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

	if (Argument_Lenght == 0)
	{
		return false;
	}

	// Add null termination.
	char* Argument_Value = (char*)malloc((size_t)Argument_Lenght + 1);

	Argument_Lenght = lws_hdr_copy(this->Params.wsi, Argument_Value, Argument_Lenght + 1, lws_token_indexes::WSI_TOKEN_HTTP_URI_ARGS);

	if (Argument_Lenght == 0)
	{
		return false;
	}

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

	if (Argument_Lenght == 0)
	{
		return false;
	}

	FString ArgumentString;
	ArgumentString.AppendChars(Argument, Argument_Lenght);

	Value = ArgumentString;

	return true;
}