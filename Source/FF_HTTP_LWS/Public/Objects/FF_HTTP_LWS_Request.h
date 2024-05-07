#pragma once

#include "Objects/FF_HTTP_LWS_Object.h"
#include "FF_HTTP_LWS_Request.generated.h"

USTRUCT()
struct FF_HTTP_LWS_API FLwsContentType
{
	GENERATED_BODY()

public:

	unsigned char* Text;
	int Lenght;
};

UCLASS(BlueprintType)
class FF_HTTP_LWS_API ULwsRequest : public ULwsObject
{
	GENERATED_BODY()

private:

	virtual FLwsContentType ContentTypeString(ELwsContentType In_Type);
	virtual int StatusEnumToInt(ELwsResponseStatus In_Status);
	virtual lws_token_indexes BpHeadersToLws(ELwsKnownHeaders HeaderEnum);
	virtual int CalculcatePayloadSize(const FString In_Response, TMap<FString, FString> In_Custom_Headers, TMap<ELwsKnownHeaders, FString> In_Known_Headers, ELwsContentType In_Type, ELwsResponseStatus In_Status);

public:

	int RetVal = 0;

	UFUNCTION(BlueprintPure, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetUri(FString& Out_Uri);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "It automatically adds \"access-control-allow-origin:*\" header."), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool SendResponse(int32& Payload_Size, const FString In_Response, TMap<FString, FString> In_Custom_Headers, TMap<ELwsKnownHeaders, FString> In_Known_Headers, ELwsContentType In_Type = ELwsContentType::Lws_Text_Plain, ELwsResponseStatus In_Status = ELwsResponseStatus::Lws_Http_Ok);

};