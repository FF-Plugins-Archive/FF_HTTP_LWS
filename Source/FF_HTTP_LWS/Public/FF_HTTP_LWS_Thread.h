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

struct LwsPoints
{
	char* Data = nullptr;
	int32 Lenght = 0;
};

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

	virtual LwsPoints ConvertAddress(FString Address, bool bMakePlatformFileName = true);
	
	virtual void Init_DynamicMount();
	virtual void Init_StaticMount();
	virtual void Init_Protocols();
	virtual void Init_Info();

	virtual void HTTP_Start();
	virtual void HTTP_Stop();
	
	int32 Port_HTTP = 8081;
	int32 Port_HTTPS = 8443;

	LwsPoints Origin;
	LwsPoints MountPoint;
	LwsPoints Page_Default;
	LwsPoints Page_Error;
	LwsPoints ApiUri;

	lws_http_mount Mounts_Static;
	lws_http_mount Mount_Dynamic;
	lws_protocols* Protocols = nullptr;
	lws_context_creation_info Info;
	lws_context* Context = nullptr;

private:

	FRunnableThread* RunnableThread = nullptr;
	bool bStartThread = false;

};