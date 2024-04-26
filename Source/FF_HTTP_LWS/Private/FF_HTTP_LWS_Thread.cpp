// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_HTTP_LWS_Thread.h"
#include "FF_HTTP_LWS_Server.h"

// UE Includes.
#include "Async/Async.h"
#include "Kismet/BlueprintPathsLibrary.h"

FHTTP_Thread_LibWebSocket::FHTTP_Thread_LibWebSocket(AHTTP_Server_LWS* In_Parent_Actor)
{
	this->Parent_Actor = In_Parent_Actor;
	
	this->Port_HTTP = this->Parent_Actor->Port_HTTP;
	this->Port_HTTPS = this->Parent_Actor->Port_HTTPS;
	this->MountPoint = this->ConvertAddress("/", false);
	this->Page_Default = this->ConvertAddress("index.html", false);

	if (!this->Parent_Actor->Server_Path_Root.IsEmpty())
	{
		this->Origin = this->ConvertAddress(this->Parent_Actor->Server_Path_Root, true);
	}

	if (!this->Parent_Actor->Server_Path_404.IsEmpty())
	{
		FString ErrorString = "/" + this->Parent_Actor->Server_Path_404;
		this->Page_Error = this->ConvertAddress(ErrorString, false);
	}

	if (!this->Parent_Actor->API_URI.IsEmpty())
	{
		this->ApiUri = this->ConvertAddress(this->Parent_Actor->API_URI, false);
	}

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
	this->HTTP_Start();
	this->bStartThread = true;

	return true;
}

uint32 FHTTP_Thread_LibWebSocket::Run()
{
	while (this->bStartThread)
	{
		if (this->Context != NULL)
		{
			lws_service(this->Context, 0);
		}
	}

	return 0;
}

void FHTTP_Thread_LibWebSocket::Stop()
{
	this->HTTP_Stop();
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

LwsPoints FHTTP_Thread_LibWebSocket::ConvertAddress(FString Address, bool bMakePlatformFileName)
{
	if (bMakePlatformFileName)
	{
		FPaths::MakePlatformFilename(Address);
	}

	LwsPoints ReturnValue;

	ReturnValue.Lenght = Address.Len();

	int32 DataSize = StringCast<TCHAR>(Address.GetCharArray().GetData()).Length() + 1;
	const wchar_t* SourceChars = StringCast<TCHAR>(Address.GetCharArray().GetData()).Get();

	ReturnValue.Data = new char[DataSize];
	size_t numCharsRead;
	wcstombs_s(&numCharsRead, ReturnValue.Data, DataSize, SourceChars, _TRUNCATE);

	return ReturnValue;
}

void FHTTP_Thread_LibWebSocket::Init_DynamicMount()
{
	memset(&this->Mount_Dynamic, 0, sizeof(lws_http_mount));

	this->Mount_Dynamic.mount_next = NULL;
	this->Mount_Dynamic.mountpoint_len = this->ApiUri.Lenght;
	this->Mount_Dynamic.mountpoint = this->ApiUri.Data;
	this->Mount_Dynamic.def = NULL;

	this->Mount_Dynamic.protocol = "http";
	this->Mount_Dynamic.origin = NULL;
	this->Mount_Dynamic.origin_protocol = LWSMPRO_CALLBACK;
	
	this->Mount_Dynamic.basic_auth_login_file = NULL;

	this->Mount_Dynamic.extra_mimetypes = NULL;
	this->Mount_Dynamic.interpret = NULL;

	this->Mount_Dynamic.cgienv = NULL;
	this->Mount_Dynamic.cgi_timeout = 0;

	this->Mount_Dynamic.cache_max_age = 0;
	this->Mount_Dynamic.cache_reusable = 0;
	this->Mount_Dynamic.cache_revalidate = 0;
	this->Mount_Dynamic.cache_intermediaries = 0;
}

void FHTTP_Thread_LibWebSocket::Init_StaticMount()
{
	memset(&this->Mounts_Static, 0, sizeof(lws_http_mount));

	this->Mounts_Static.mount_next = &this->Mount_Dynamic;
	this->Mounts_Static.mountpoint_len = this->MountPoint.Lenght;

	this->Mounts_Static.mountpoint = this->MountPoint.Data;
	this->Mounts_Static.def = this->Page_Default.Data;

	this->Mounts_Static.protocol = NULL;
	this->Mounts_Static.origin = this->Origin.Data;
	this->Mounts_Static.origin_protocol = LWSMPRO_FILE;

	this->Mounts_Static.auth_mask = 0;
	this->Mounts_Static.basic_auth_login_file = NULL;

	this->Mounts_Static.extra_mimetypes = NULL;
	this->Mounts_Static.interpret = NULL;

	this->Mounts_Static.cgienv = NULL;
	this->Mounts_Static.cgi_timeout = 0;

	this->Mounts_Static.cache_max_age = 0;
	this->Mounts_Static.cache_reusable = 0;
	this->Mounts_Static.cache_revalidate = 0;
	this->Mounts_Static.cache_intermediaries = 0;
}

int FHTTP_Thread_LibWebSocket::Callback_HTTP(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len)
{
	FHTTP_Thread_LibWebSocket* Owner = (FHTTP_Thread_LibWebSocket*)lws_context_user(lws_get_context(wsi));
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("LibWebSocket = HTTP Request catched but \"Owner\" is NULL !"));
		return lws_callback_http_dummy(wsi, reason, user, in, len);
	}

	if (reason == LWS_CALLBACK_HTTP)
	{
		UHttpConnectionLws* HttpConnection = NewObject<UHttpConnectionLws>();
		HttpConnection->CallbackParams.wsi = wsi;
		HttpConnection->CallbackParams.reason = reason;
		HttpConnection->CallbackParams.user = user;
		HttpConnection->CallbackParams.in = in;
		HttpConnection->CallbackParams.len = len;

		Owner->Parent_Actor->DelegateHttpMessageAdv.Broadcast(HttpConnection);
	}

	return lws_callback_http_dummy(wsi, reason, user, in, len);
}

void FHTTP_Thread_LibWebSocket::Init_Protocols()
{
	this->Protocols = new lws_protocols[2];

	// HTTP

	this->Protocols[0].name = "http";
	this->Protocols[0].callback = FHTTP_Thread_LibWebSocket::Callback_HTTP;
	this->Protocols[0].rx_buffer_size = 0;
	this->Protocols[0].per_session_data_size = 0;
	
	// Destructor.

	this->Protocols[1].name = nullptr;
	this->Protocols[1].callback = nullptr;
	this->Protocols[1].per_session_data_size = 0;
}

void FHTTP_Thread_LibWebSocket::Init_Info()
{
	memset(&this->Info, 0, sizeof(lws_context_creation_info));
	this->Info.error_document_404 = this->Page_Error.Data;
	this->Info.mounts = &this->Mounts_Static;
	this->Info.port = this->Port_HTTP;
	this->Info.protocols = &this->Protocols[0];
	this->Info.user = (void*)this;
	this->Info.gid = -1;
	this->Info.uid = -1;
	this->Info.options = LWS_SERVER_OPTION_ALLOW_LISTEN_SHARE;
}

void FHTTP_Thread_LibWebSocket::HTTP_Start()
{
	this->Init_DynamicMount();
	this->Init_StaticMount();
	this->Init_Protocols();
	this->Init_Info();

	this->Context = lws_create_context(&this->Info);
}

void FHTTP_Thread_LibWebSocket::HTTP_Stop()
{
	if (this->Context)
	{
		lws_context_destroy(this->Context);
		this->Context = NULL;
	}

	this->Protocols = nullptr;
}