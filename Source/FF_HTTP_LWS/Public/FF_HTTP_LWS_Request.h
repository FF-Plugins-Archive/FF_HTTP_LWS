#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#ifdef _WIN64
#define UI UI_ST
#include "Libwebsockets.h"
#undef UI
#endif
THIRD_PARTY_INCLUDES_END

#include "FF_HTTP_LWS_Request.generated.h"

USTRUCT(BlueprintType)
struct FF_HTTP_LWS_API FLibWebSocketCallbackParams
{
	GENERATED_BODY()

public:

	lws* wsi = nullptr;
	void* user = nullptr;
	void* in = nullptr;
	lws_callback_reasons reason;
	size_t len;

};

UCLASS()
class FF_HTTP_LWS_API UHttpConnectionLws : public UObject
{
	GENERATED_BODY()

private:

	virtual FString ParseReceived();

public:

	UPROPERTY(BlueprintReadOnly)
	FLibWebSocketCallbackParams CallbackParams;

	UFUNCTION(BlueprintPure, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetUri(FString& Out_Uri);

	UFUNCTION(BlueprintPure, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetBody(FString& Out_Body);

	UFUNCTION(BlueprintPure, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetHeader(FString& Value, FString Key);

	UFUNCTION(BlueprintPure, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetParams(TMap<FString, FString>& Out_Params);

	UFUNCTION(BlueprintCallable, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool SendResponse(const FString In_Response, TMap<FString, FString> In_Header, const bool bAddAllowOrigin = true, int32 Status_Code = 200);

};