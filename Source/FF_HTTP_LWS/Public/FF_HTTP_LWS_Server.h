// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Threads.
#include "FF_HTTP_LWS_Thread.h"

#include "FF_HTTP_LWS_Server.generated.h"

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

public:

	// Define this in server's FRunnableThread.
	FLibWebSocketCallbackParams CallbackParams;

	UFUNCTION(BlueprintCallable, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool ParseRequest()
	{
		if (!this->CallbackParams.wsi)
		{
			return false;
		}

		return true;
	}

	UFUNCTION(BlueprintCallable, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool SendResponse(const FString In_Response, TMap<FString, FString> In_Header, const bool bAddAllowOrigin = true, int32 Status_Code = 200)
	{
#ifdef _WIN64

		if (!this->CallbackParams.wsi)
		{
			return false;
		}

		return true;

#else
		return false;
#endif
	}

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateLibWebSocket, UHttpConnectionLws*, Connection);

UCLASS()
class FF_HTTP_LWS_API AHTTP_Server_LWS : public AActor
{
	GENERATED_BODY()
	
protected:

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the game end or when destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	

	// Sets default values for this actor's properties.
	AHTTP_Server_LWS();

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

#ifdef _WIN64
	class FHTTP_Thread_LibWebSocket* Thread_LibWebSocket = nullptr;
#endif

	UFUNCTION(BlueprintImplementableEvent, meta = (Description = ""), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	void OnHttpAdvStart();

	UFUNCTION(BlueprintImplementableEvent, meta = (Description = ""), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	void OnHttpAdvStop();

	UFUNCTION(BlueprintImplementableEvent, meta = (Description = ""), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	void OnHttpAdvMessage(UHttpConnectionLws* Connection);

	UPROPERTY(BlueprintAssignable, Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	FDelegateLibWebSocket DelegateHttpMessageAdv;

public:

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "", ExposeOnSpawn = "true"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	FString Server_Path_Root = "";

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "", ExposeOnSpawn = "true"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	FString Server_Path_404 = "";

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "", ExposeOnSpawn = "true"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	int32 Port_HTTP = 8081;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "", ExposeOnSpawn = "true"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	int32 Port_HTTPS = 8453;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "If you want to change API parameter, just put /* to the end. If you don't do that, server won't detect dynamic API requests.", ExposeOnSpawn = "true"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	FString API_URI = "api/*";

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "It shouldn't bigger than 15 chars and it has to be unique.", ExposeOnSpawn = "true"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	FString Server_Name = "";

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HTTP Server LibWebSocket - Start"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool HTTP_Server_Start();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HTTP Server LibWebSocket - Stop"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual void HTTP_Server_Stop();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HTTP Server LibWebSocket - Toggle"), Category = "Frozen Forest|HTTP|Server|LibWebSocket")
	virtual bool HTTP_Server_Toggle(bool bIsPause);

};