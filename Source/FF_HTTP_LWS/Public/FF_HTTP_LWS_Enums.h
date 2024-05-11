#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ELwsKnownHeaders : uint8
{
	NONE										UMETA(DisplayName = "LWS NONE"),
	Lws_Header_X_Auth_Token						UMETA(DisplayName = "LWS Header	X-Auth Token"),
	Lws_Header_Authorization					UMETA(DisplayName = "LWS Header	Authorization"),
	Lws_Header_WWW_Authorization				UMETA(DisplayName = "LWS Header	WWW Auth"),
	Lws_Header_Proxy_Authenticate				UMETA(DisplayName = "LWS Header	Proxy Authenticate"),
	Lws_Header_Proxy_Authorization				UMETA(DisplayName = "LWS Header	Proxy Authorization"),
	
	Lws_Header_Accept							UMETA(DisplayName = "LWS Header Accept"),
	Lws_Header_Accept_Encoding					UMETA(DisplayName = "LWS Header Accept Encoding"),
	
	Lws_Header_Content_Type						UMETA(DisplayName = "LWS Header Content Type"),
	Lws_Header_Content_Lenght					UMETA(DisplayName = "LWS Header Content Lenght"),
	
	Lws_Header_Host								UMETA(DisplayName = "LWS Header Host"),
	Lws_Header_Origin							UMETA(DisplayName = "LWS Header Origin"),
	Lws_Header_Connection						UMETA(DisplayName = "LWS Header Connection"),
	Lws_Header_User_Agent						UMETA(DisplayName = "LWS Header User Agent"),
	Lws_Header_Access_Control_Origin			UMETA(DisplayName = "LWS Header	Access Control Origin"),
	
};
ENUM_CLASS_FLAGS(ELwsKnownHeaders)

UENUM(BlueprintType)
enum class ELwsResponseStatus : uint8
{
	Lws_Http_Continue							UMETA(DisplayName = "LWS HTTP Status Continue (100)"),
	Lws_Http_Ok									UMETA(DisplayName = "LWS HTTP Status Ok (200)"),
	Lws_Http_No_Content							UMETA(DisplayName = "LWS HTTP Status No Content (204)"),
	Lws_Http_Partial_Content					UMETA(DisplayName = "LWS HTTP Status Partial Content (206)"),
	Lws_Http_Bad_Request						UMETA(DisplayName = "LWS HTTP Status Bad Request (400)"),
	Lws_Http_Not_Found							UMETA(DisplayName = "LWS HTTP Status Not Found (404)"),
	Lws_Http_Int_Server_Error					UMETA(DisplayName = "LWS HTTP Status Internal Server Error (500)"),

};
ENUM_CLASS_FLAGS(ELwsResponseStatus)

UENUM(BlueprintType)
enum class ELwsContentType : uint8
{
	Lws_Application_Json						UMETA(DisplayName = "LWS Application/Json"),
	Lws_Text_Plain								UMETA(DisplayName = "LWS Text/Plain"),
	Lws_Text_Html								UMETA(DisplayName = "LWS Text/HTML"),
};
ENUM_CLASS_FLAGS(ELwsContentType)