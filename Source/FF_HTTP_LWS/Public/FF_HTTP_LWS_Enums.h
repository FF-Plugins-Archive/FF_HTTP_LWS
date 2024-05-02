#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ELwsReasons : uint8
{
	Lws_Callback_None							UMETA(DisplayName = "Lws_Callback_None"),
	Lws_Callback_Http							UMETA(DisplayName = "LWS Callback HTTP"),
	Lws_Callback_Http_Body						UMETA(DisplayName = "Lws_Callback_Http_Body"),
	Lws_Callback_Http_Body_Completion			UMETA(DisplayName = "Lws_Callback_Http_Body_Completion"),
	Lws_Callback_Http_Writable					UMETA(DisplayName = "Lws_Callback_Http_Writable"),
};
ENUM_CLASS_FLAGS(ELwsReasons)