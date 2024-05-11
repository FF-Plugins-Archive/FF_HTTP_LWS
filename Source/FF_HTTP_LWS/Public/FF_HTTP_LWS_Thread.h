// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

THIRD_PARTY_INCLUDES_START
#define UI UI_ST
#include "Libwebsockets.h"
#undef UI
THIRD_PARTY_INCLUDES_END

#include "Objects/FF_HTTP_LWS_Info.h"
#include "Objects/FF_HTTP_LWS_Request.h"

#define USE_DISCARDABLE_CACHE 0
#if USE_DISCARDABLE_CACHE == 1
#include "Containers/DiscardableKeyValueCache.h"
#endif

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

	static int Callback_HTTP(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len);
	
	// Exprimental.
	virtual int Callback_Return(lws* wsi);

#if USE_DISCARDABLE_CACHE == 0
	
	mutable FCriticalSection Section_Pool_Add;
	mutable FCriticalSection Section_Pool_Remove;
	TMap<lws*, ULwsRequest*> RequestPool;

#else
	TDiscardableKeyValueCache<lws*, ULwsRequest*> RequestCache;
#endif

private:

	virtual LwsPoints ConvertAddress(FString Address, bool bMakePlatformFileName = true);

	LwsPoints Origin;
	LwsPoints MountPoint;
	LwsPoints Page_Default;
	LwsPoints Page_Error;
	LwsPoints ApiUri;

private:

	FRunnableThread* RunnableThread = nullptr;
	bool bStartThread = false;

	virtual void Init_DynamicMount();
	virtual void Init_StaticMount();
	virtual void Init_Protocols();
	virtual void Init_Info();

	// Main Functions.

	virtual void HTTP_Start();
	virtual void HTTP_Stop();
	
	int32 Port_HTTP = 8081;
	int32 Port_HTTPS = 8443;

	lws_http_mount Mount_Static;
	lws_http_mount Mount_Dynamic;
	lws_protocols* Protocols = nullptr;
	lws_context_creation_info Info;
	lws_context* Context = nullptr;

};