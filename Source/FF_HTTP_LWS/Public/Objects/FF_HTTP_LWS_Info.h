#pragma once

#include "FF_HTTP_LWS_Object.h"
#include "FF_HTTP_LWS_Info.generated.h"

UCLASS(BlueprintType)
class FF_HTTP_LWS_API ULwsInfos : public ULwsObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool GetBody(FString& Out_Body);

};