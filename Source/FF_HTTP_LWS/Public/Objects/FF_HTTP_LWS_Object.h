#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#ifdef _WIN64
#define UI UI_ST
#include "Libwebsockets.h"
#undef UI
#endif
THIRD_PARTY_INCLUDES_END

// Custom Includes.
#include "FF_HTTP_LWS_Enums.h"

#include "FF_HTTP_LWS_Object.generated.h"

USTRUCT()
struct FF_HTTP_LWS_API FLwsParams
{
	GENERATED_BODY()

public:

	lws* wsi = nullptr;
	void* in = nullptr;
	void* user = nullptr;
	lws_callback_reasons reason;
	size_t len;
};

USTRUCT(BlueprintType)
struct FF_HTTP_LWS_API FLwsKnownHeaders
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FString String;

	UPROPERTY(BlueprintReadOnly)
	int32 Index = 0;
};

UCLASS()
class FF_HTTP_LWS_API ULwsObject : public UObject
{
	GENERATED_BODY()

public:

	// This is HTTP Request params for LibWebSocket library. Not mistake it with URL params !
	FLwsParams Params;

	UFUNCTION(BlueprintPure, meta = (Tooltip = "", AdvancedDisplay = ""), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetMethod(FString& Out_Method);

	UFUNCTION(BlueprintPure, meta = (Tooltip = "", AdvancedDisplay = ""), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetCustomHeader(FString& Value, FString Key);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "", AdvancedDisplay = ""), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetAllCustomHeaders(TArray<FString> In_Headers, TMap<FString, FString>& Out_Headers);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "", AdvancedDisplay = ""), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetAllKnownHeaders(TMap<FString, FLwsKnownHeaders>& Out_Headers);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "", AdvancedDisplay = ""), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetAllUrlParameters(TMap<FString, FString>& Out_Params);

	UFUNCTION(BlueprintPure, meta = (Tooltip = "Change buffer size if you only need to get longer values than 1024 chars.", AdvancedDisplay = "BufferSize"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetUrlParam(FString& Value, FString Key, int32 BufferSize = 1024);

	UFUNCTION(BlueprintPure, meta = (Tooltip = "", AdvancedDisplay = ""), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual ELwsCallbacks GetReason();

};