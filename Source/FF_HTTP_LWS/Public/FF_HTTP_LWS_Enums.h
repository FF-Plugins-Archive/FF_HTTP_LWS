#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ELwsCallbacks : uint8
{
	Lws_Callback_None							UMETA(DisplayName = "Lws_Callback_None"),
	Lws_Callback_Http							UMETA(DisplayName = "LWS Callback HTTP"),
	Lws_Callback_Http_Body						UMETA(DisplayName = "Lws_Callback_Http_Body"),
	Lws_Callback_Http_Body_Completion			UMETA(DisplayName = "Lws_Callback_Http_Body_Completion"),
	Lws_Callback_Http_Writable					UMETA(DisplayName = "Lws_Callback_Http_Writable"),
};
ENUM_CLASS_FLAGS(ELwsCallbacks)

UENUM(BlueprintType)
enum class ELwsKnownHeaders : uint8
{
	Lws_Header_Origin							UMETA(DisplayName = "Lws Header Origin"),
	Lws_Header_Content_Lenght					UMETA(DisplayName = "Lws Header Content Lenght"),
	Lws_Header_Content_Type						UMETA(DisplayName = "Lws Header Content Type"),
	Lws_Header_Host								UMETA(DisplayName = "Lws Header Host"),
	Lws_Header_User_Agent						UMETA(DisplayName = "Lws Header User Agent"),
	Lws_Header_Accept							UMETA(DisplayName = "Lws Header Accept"),
	Lws_Header_Accept_Encoding					UMETA(DisplayName = "Lws Header Accept Encoding"),
	Lws_Header_Connection						UMETA(DisplayName = "Lws Header Connection"),
	Lws_Header_Authentication					UMETA(DisplayName = "Lws Header	Authentication"),

};
ENUM_CLASS_FLAGS(ELwsKnownHeaders)