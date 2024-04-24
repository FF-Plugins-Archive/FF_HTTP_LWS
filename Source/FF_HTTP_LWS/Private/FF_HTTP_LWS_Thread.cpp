// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_HTTP_LWS_Thread.h"
#include "FF_HTTP_LWS_Server.h"

// UE Includes.
#include "Async/Async.h"

FHTTP_Thread_LibWebSocket::FHTTP_Thread_LibWebSocket(AHTTP_Server_LWS* In_Parent_Actor)
{
	this->Parent_Actor = In_Parent_Actor;
	
	this->Port_HTTP = this->Parent_Actor->Port_HTTP;
	this->Port_HTTPS = this->Parent_Actor->Port_HTTPS;

	this->RunnableThread = FRunnableThread::Create(this, *this->Parent_Actor->Server_Name);
}

FHTTP_Thread_LibWebSocket::~FHTTP_Thread_LibWebSocket()
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Kill(true);
		delete this->RunnableThread;
	}
}

bool FHTTP_Thread_LibWebSocket::Init()
{	
	this->Callback_HTTP_Start();
	this->bStartThread = true;

	return true;
}

uint32 FHTTP_Thread_LibWebSocket::Run()
{
	while (this->bStartThread)
	{
		if (this->Server_Context)
		{
			lws_service(this->Server_Context, 0);
		}
	}

	return 0;
}

void FHTTP_Thread_LibWebSocket::Stop()
{
	this->Callback_HTTP_Stop();
	this->bStartThread = false;
}

bool FHTTP_Thread_LibWebSocket::Toggle(bool bIsPause)
{
	if (!this->RunnableThread)
	{
		return false;
	}

	this->RunnableThread->Suspend(bIsPause);
	
	return true;
}

void FHTTP_Thread_LibWebSocket::Callback_LibWebSocket_Mount_Dyn()
{
	memset(&this->Server_Mount_Dynamic, 0, sizeof(this->Server_Mount_Dynamic));
	
	this->Server_Mount_Dynamic.mountpoint = TCHAR_TO_UTF8(*this->Parent_Actor->API_URI);
	this->Server_Mount_Dynamic.protocol = "http";
	this->Server_Mount_Dynamic.origin_protocol = LWSMPRO_CALLBACK;
	this->Server_Mount_Dynamic.mountpoint_len = 4;

	this->Server_Mount_Dynamic.mount_next = NULL;
	this->Server_Mount_Dynamic.origin = NULL;
	this->Server_Mount_Dynamic.def = NULL;
	this->Server_Mount_Dynamic.cgienv = NULL;
	this->Server_Mount_Dynamic.basic_auth_login_file = NULL;
	this->Server_Mount_Dynamic.extra_mimetypes = NULL;
	this->Server_Mount_Dynamic.interpret = NULL;

	this->Server_Mount_Dynamic.cgi_timeout = 0;
	this->Server_Mount_Dynamic.cache_max_age = 0;
	this->Server_Mount_Dynamic.cache_reusable = 0;
	this->Server_Mount_Dynamic.cache_revalidate = 0;
	this->Server_Mount_Dynamic.cache_intermediaries = 0;
}

void FHTTP_Thread_LibWebSocket::Callback_LibWebSocket_Mount_Static()
{
	memset(&this->Server_Mount_Static, 0, sizeof(this->Server_Mount_Dynamic));

	this->Server_Mount_Static.mount_next = &this->Server_Mount_Dynamic;
	this->Server_Mount_Static.mountpoint = "/";
	this->Server_Mount_Static.origin = TCHAR_TO_UTF8(*this->Parent_Actor->Server_Path_Root);
	this->Server_Mount_Static.def = "index.html";
	this->Server_Mount_Static.origin_protocol = LWSMPRO_FILE;
	this->Server_Mount_Static.mountpoint_len = 1;

	this->Server_Mount_Static.cgienv = NULL;
	this->Server_Mount_Static.basic_auth_login_file = NULL;
	this->Server_Mount_Static.extra_mimetypes = NULL;
	this->Server_Mount_Static.interpret = NULL;

	this->Server_Mount_Static.cgi_timeout = 0;
	this->Server_Mount_Static.cache_max_age = 0;
	this->Server_Mount_Static.cache_reusable = 0;
	this->Server_Mount_Static.cache_revalidate = 0;
	this->Server_Mount_Static.cache_intermediaries = 0;
}

void FHTTP_Thread_LibWebSocket::Callback_HTTP_Start()
{
	auto Callback_HTTP = [](lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len)->int
		{
			lws_spa* spa = nullptr;

			const char* param_names[] =
			{
				"text1",
				"send",
			};

			int n = 0;

			if (reason == LWS_CALLBACK_HTTP_BODY)
			{
				
				spa = lws_spa_create(wsi, param_names, LWS_ARRAY_SIZE(param_names), 1024, NULL, NULL);
				lws_spa_process(spa, (const char*)in, len);
				const char* ResultString = lws_spa_get_string(spa, n);

				UE_LOG(LogTemp, Warning, TEXT("LibWebSocket SPA String : %s"), ResultString);
			}

			if (reason == LWS_CALLBACK_HTTP)
			{
				FHTTP_Thread_LibWebSocket* Owner = (FHTTP_Thread_LibWebSocket*)lws_context_user(lws_get_context(wsi));
				if (!Owner)
				{
					UE_LOG(LogTemp, Warning, TEXT("LibWebSocket = HTTP Request catched but \"Owner\" is NULL !"));
					return 1;
				}
				
				UHttpConnectionLws* HttpConnection = NewObject<UHttpConnectionLws>();
				HttpConnection->CallbackParams.wsi = wsi;
				HttpConnection->CallbackParams.reason = reason;
				HttpConnection->CallbackParams.user = user;
				HttpConnection->CallbackParams.in = in;
				HttpConnection->CallbackParams.len = len;

				Owner->Parent_Actor->DelegateHttpMessageAdv.Broadcast(HttpConnection);
			}

			return lws_callback_http_dummy(wsi, reason, user, in, len);
		};

	this->Callback_LibWebSocket_Mount_Dyn();
	this->Callback_LibWebSocket_Mount_Static();

	this->Server_Protocols.SetNum(2);
	this->Server_Protocols[0].name = "http";
	this->Server_Protocols[0].callback = Callback_HTTP;
	this->Server_Protocols[0].per_session_data_size = 0;
	this->Server_Protocols[0].rx_buffer_size = 0;
	
	this->Server_Protocols[1].name = NULL;
	this->Server_Protocols[1].callback = NULL;
	this->Server_Protocols[1].per_session_data_size = 0;
	this->Server_Protocols[1].rx_buffer_size = 0;

	memset(&this->Server_Info, 0, sizeof(this->Server_Info));
	//this->Server_Info.error_document_404 = TCHAR_TO_UTF8(*this->Parent_Actor->Server_Path_404);
	this->Server_Info.mounts = &this->Server_Mount_Static;
	this->Server_Info.port = this->Port_HTTP;
	this->Server_Info.protocols = this->Server_Protocols.GetData();
	this->Server_Info.user = (void*)this;
	this->Server_Info.uid = -1;
	this->Server_Info.gid = -1;

	this->Server_Context = lws_create_context(&this->Server_Info);
}

void FHTTP_Thread_LibWebSocket::Callback_HTTP_Stop()
{
	if (this->Server_Context)
	{
		lws_context_destroy(this->Server_Context);
		this->Server_Context = NULL;
	}

	if (!this->Server_Protocols.IsEmpty())
	{
		this->Server_Protocols.Empty();
	}
}