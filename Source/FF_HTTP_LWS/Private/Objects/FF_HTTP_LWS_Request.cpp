#include "Objects/FF_HTTP_LWS_Request.h"

// Hellper Functions.

FLwsContentType ULwsRequest::ContentTypeString(ELwsContentType In_Type)
{
	FLwsContentType Info;
	memset(&Info, 0, sizeof(FLwsContentType));

	switch (In_Type)
	{
		case ELwsContentType::Lws_Text_Plain:

			Info.Text = (unsigned char*)"text/plain";
			Info.Lenght = 10;
			return Info;

		case ELwsContentType::Lws_Text_Html:

			Info.Text = (unsigned char*)"text/html";
			Info.Lenght = 9;
			return Info;

		case ELwsContentType::Lws_Application_Json:

			Info.Text = (unsigned char*)"application/json";
			Info.Lenght = 16;
			return Info;

		default:

			Info.Text = (unsigned char*)"text/plain";
			Info.Lenght = 10;
			return Info;
		}
}

int ULwsRequest::StatusEnumToInt(ELwsResponseStatus In_Status)
{
	switch (In_Status)
	{
	case ELwsResponseStatus::Lws_Http_Continue:
		return 100;
	case ELwsResponseStatus::Lws_Http_Ok:
		return 200;
	case ELwsResponseStatus::Lws_Http_No_Content:
		return 204;
	case ELwsResponseStatus::Lws_Http_Partial_Content:
		return 206;
	case ELwsResponseStatus::Lws_Http_Bad_Request:
		return 400;
	case ELwsResponseStatus::Lws_Http_Not_Found:
		return 404;
	case ELwsResponseStatus::Lws_Http_Int_Server_Error:
		return 500;
	default:
		return 404;
	}
}

lws_token_indexes ULwsRequest::BpHeadersToLws(ELwsKnownHeaders HeaderEnum)
{
	switch (HeaderEnum)
	{
		case ELwsKnownHeaders::Lws_Header_X_Auth_Token:
			return lws_token_indexes::WSI_TOKEN_X_AUTH_TOKEN;

		case ELwsKnownHeaders::Lws_Header_Authorization:
			return lws_token_indexes::WSI_TOKEN_HTTP_AUTHORIZATION;
		
		case ELwsKnownHeaders::Lws_Header_WWW_Authorization:
			return lws_token_indexes::WSI_TOKEN_HTTP_WWW_AUTHENTICATE;
		
		case ELwsKnownHeaders::Lws_Header_Proxy_Authenticate:
			return lws_token_indexes::WSI_TOKEN_HTTP_PROXY_AUTHENTICATE;

		case ELwsKnownHeaders::Lws_Header_Proxy_Authorization:
			return lws_token_indexes::WSI_TOKEN_HTTP_PROXY_AUTHORIZATION;
		
		case ELwsKnownHeaders::Lws_Header_Accept:
			return lws_token_indexes::WSI_TOKEN_HTTP_ACCEPT;
		
		case ELwsKnownHeaders::Lws_Header_Accept_Encoding:
			return lws_token_indexes::WSI_TOKEN_HTTP_ACCEPT_ENCODING;
		
		case ELwsKnownHeaders::Lws_Header_Content_Type:
			return lws_token_indexes::WSI_TOKEN_HTTP_CONTENT_TYPE;
		
		case ELwsKnownHeaders::Lws_Header_Content_Lenght:
			return lws_token_indexes::WSI_TOKEN_HTTP_CONTENT_LENGTH;
		
		case ELwsKnownHeaders::Lws_Header_Host:
			return lws_token_indexes::WSI_TOKEN_HOST;
		
		case ELwsKnownHeaders::Lws_Header_Origin:
			return lws_token_indexes::WSI_TOKEN_ORIGIN;
		
		case ELwsKnownHeaders::Lws_Header_Connection:
			return lws_token_indexes::WSI_TOKEN_CONNECTION;
		
		case ELwsKnownHeaders::Lws_Header_User_Agent:
			return lws_token_indexes::WSI_TOKEN_HTTP_USER_AGENT;
		
		case ELwsKnownHeaders::Lws_Header_Access_Control_Origin:
			return lws_token_indexes::WSI_TOKEN_HTTP_ACCESS_CONTROL_ALLOW_ORIGIN;
		
		default:
			return lws_token_indexes::WSI_TOKEN_HTTP_CONTENT_TYPE;
	}
}

int ULwsRequest::CalculcatePayloadSize(const FString In_Response, TMap<FString, FString> In_Custom_Headers, TMap<ELwsKnownHeaders, FString> In_Known_Headers, ELwsContentType In_Type, ELwsResponseStatus In_Status)
{
	// All response elements has two termination string (one white space and one line breaking). Last header only has white space.
	const int TERMINATION = 2;

	int Lenght_Response = strlen((const char*)lws_token_to_string(lws_token_indexes::WSI_TOKEN_HTTP_CONTENT_LENGTH)) + FString::FromInt(In_Response.Len()).Len() + In_Response.Len() + TERMINATION;

	int Lenght_Headers_Custom = 0;
	for (TPair EachHeader : In_Custom_Headers)
	{
		Lenght_Headers_Custom += (EachHeader.Key + FString(":")).Len();
		Lenght_Headers_Custom += EachHeader.Value.Len() + TERMINATION;
	}

	int Lenght_Headers_Known = 0;
	for (TPair EachHeader : In_Known_Headers)
	{
		Lenght_Headers_Known += strlen((const char*)lws_token_to_string(this->BpHeadersToLws(EachHeader.Key)));
		Lenght_Headers_Known += EachHeader.Value.Len() + TERMINATION;
	}

	int Lenght_Header_AO = strlen((const char*)lws_token_to_string(lws_token_indexes::WSI_TOKEN_HTTP_ACCESS_CONTROL_ALLOW_ORIGIN)) + strlen("*") + TERMINATION;

	int Lenght_Header_CT = strlen((const char*)lws_token_to_string(lws_token_indexes::WSI_TOKEN_HTTP_CONTENT_TYPE)) + this->ContentTypeString(In_Type).Lenght + TERMINATION/2;

	int Total = Lenght_Response + Lenght_Headers_Custom + Lenght_Headers_Known + Lenght_Header_AO + Lenght_Header_CT;

	return Total;
}

// Blueprint Functions.

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

bool ULwsRequest::SendResponse(int32& Payload_Size, const FString In_Response, TMap<FString, FString> In_Custom_Headers, TMap<ELwsKnownHeaders, FString> In_Known_Headers, ELwsContentType In_Type, ELwsResponseStatus In_Status)
{
	Payload_Size = this->CalculcatePayloadSize(In_Response, In_Custom_Headers, In_Known_Headers, In_Type, In_Status);

	unsigned char* Response_Buffer = (unsigned char*)malloc((size_t)Payload_Size + LWS_PRE);
	unsigned char* Position = &Response_Buffer[LWS_PRE];
	unsigned char* End = &Response_Buffer[Payload_Size];

	lws_add_http_header_status(this->Params.wsi, HTTP_STATUS_OK, &Position, End);
	lws_add_http_header_by_token(this->Params.wsi, lws_token_indexes::WSI_TOKEN_HTTP_ACCESS_CONTROL_ALLOW_ORIGIN, (unsigned char*)"*", 1, &Position, End);

	FLwsContentType ContentType = this->ContentTypeString(In_Type);
	lws_add_http_header_by_token(this->Params.wsi, WSI_TOKEN_HTTP_CONTENT_TYPE, ContentType.Text, ContentType.Lenght, &Position, End);
	lws_add_http_header_content_length(this->Params.wsi, In_Response.Len(), &Position, End);

	for (TPair EachHeader : In_Custom_Headers)
	{
		const unsigned char* Header_Name = (const unsigned char*)TCHAR_TO_UTF8(*(EachHeader.Key + ":"));
		const unsigned char* Header_Value = (const unsigned char*)TCHAR_TO_UTF8(*EachHeader.Value);
		int Header_Value_Lenght = EachHeader.Value.Len();

		if (Header_Value_Lenght > 0)
		{
			lws_add_http_header_by_name(this->Params.wsi, Header_Name, Header_Value, Header_Value_Lenght, &Position, End);
		}
	}

	for (TPair EachHeader : In_Known_Headers)
	{
		lws_token_indexes Header_Token = this->BpHeadersToLws(EachHeader.Key);
		const unsigned char* Header_Value = (const unsigned char*)TCHAR_TO_UTF8(*EachHeader.Value);
		int Header_Value_Lenght = EachHeader.Value.Len();

		if (Header_Value_Lenght > 0)
		{
			lws_add_http_header_by_token(this->Params.wsi, Header_Token, Header_Value, Header_Value_Lenght, &Position, End);
		}
	}

	lws_finalize_http_header(this->Params.wsi, &Position, End);

	lws_write(this->Params.wsi, &Response_Buffer[LWS_PRE], Position - &Response_Buffer[LWS_PRE], LWS_WRITE_HTTP_HEADERS);
	lws_write(this->Params.wsi, (unsigned char*)TCHAR_TO_UTF8(*In_Response), In_Response.Len(), LWS_WRITE_HTTP);

	this->RetVal = lws_callback_http_dummy(this->Params.wsi, this->Params.reason, this->Params.user, this->Params.in, this->Params.len);
	return true;
}