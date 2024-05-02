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

#include "FF_HTTP_LWS_Request.generated.h"

USTRUCT(BlueprintType)
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

UCLASS()
class FF_HTTP_LWS_API ULwsObject : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FLwsParams Params;

	UFUNCTION(BlueprintPure, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetCustomHeader(FString& Value, FString Key);

};

UCLASS(BlueprintType)
class FF_HTTP_LWS_API ULwsRequest : public ULwsObject
{
	GENERATED_BODY()

public:

	int RetVal = 0;

	UFUNCTION(BlueprintPure, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetUri(FString& Out_Uri);

	UFUNCTION(BlueprintCallable, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool SendResponse(const FString In_Response, TMap<FString, FString> In_Header, const bool bAddAllowOrigin = true, int32 Status_Code = 200);

};

UCLASS(BlueprintType)
class FF_HTTP_LWS_API ULwsInfos : public ULwsObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetBody(FString& Out_Body);

};