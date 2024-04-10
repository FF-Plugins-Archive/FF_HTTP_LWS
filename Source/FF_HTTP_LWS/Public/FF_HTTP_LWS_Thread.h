// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

THIRD_PARTY_INCLUDES_START
#ifdef _WIN64
#define UI UI_ST
#include "Libwebsockets.h"
#undef UI
#endif
THIRD_PARTY_INCLUDES_END

// Fordward Declerations.
class FRunnableThread;
class AHTTP_Server_LWS;

class FHTTP_Thread_LibWebSocket : public FRunnable
{
	
public:	
	
	// Sets default values for this actor's properties
	FHTTP_Thread_LibWebSocket(AHTTP_Server_LWS* In_Parent_Actor);

	// Destructor.
	virtual ~FHTTP_Thread_LibWebSocket() override;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual bool Toggle(bool bIsPause);

	AHTTP_Server_LWS* Parent_Actor = nullptr;

private:

	virtual void Callback_HTTP_Start();
	virtual void Callback_HTTP_Stop();

	virtual void Callback_LibWebSocket_Mount_Dyn();
	virtual void Callback_LibWebSocket_Mount_Static();
	

	int32 Port_HTTP = 8081;
	int32 Port_HTTPS = 8443;

	FString Server_Address_HTTPS = "";
	FString Server_Address_HTTP = "";
	FString Server_Path_Root = "";
	FString API_URI = "";

	lws_http_mount Server_Mount_Dynamic;
	lws_http_mount Server_Mount_Static;
	lws_context_creation_info Server_Info;
	TArray<lws_protocols> Server_Protocols;
	lws_context* Server_Context;

private:

	FRunnableThread* RunnableThread = nullptr;
	bool bStartThread = false;

};