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

	if (!this->Parent_Actor->Server_Path_Root.IsEmpty())
	{
		this->Server_Path_Root = this->Parent_Actor->Server_Path_Root;
	}

	if (!this->Parent_Actor->Server_Path_404.IsEmpty())
	{
		this->Server_Path_404 = this->Parent_Actor->Server_Path_404;
	}

	if (!this->Parent_Actor->Server_Path_404.IsEmpty())
	{
		this->API_URI = this->Parent_Actor->API_URI;
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
	this->Callback_HTTP_Start();
	this->bStartThread = true;

	return true;
}

uint32 FHTTP_Thread_LibWebSocket::Run()
{
	while (this->bStartThread)
	{
		if (this->Context != NULL)
		{
			lws_service(this->Context, 10);
			//lws_callback_on_writable_all_protocol(this->Server_Context, &this->Server_Protocols[0]);
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

const char* FHTTP_Thread_LibWebSocket::ConvertAddress(int32& CharLenght, FString Address, bool bMakePlatformFileName)
{
	if (bMakePlatformFileName)
	{
		FPaths::MakePlatformFilename(Address);
	}

	CharLenght = Address.Len();

	int32 DataSize = StringCast<TCHAR>(Address.GetCharArray().GetData()).Length() + 1;
	const wchar_t* SourceChars = StringCast<TCHAR>(Address.GetCharArray().GetData()).Get();

	char* c_string = new char[DataSize];
	size_t numCharsRead;
	wcstombs_s(&numCharsRead, c_string, DataSize, SourceChars, _TRUNCATE);

	return c_string;
}

void FHTTP_Thread_LibWebSocket::Callback_HTTP_Start()
{
	int32 Lenght_Origin;
	const char* Origin = this->ConvertAddress(Lenght_Origin, this->Server_Path_Root);

	int32 Lenght_Mount;
	const char* MountPoint = this->ConvertAddress(Lenght_Mount, "/", false);

	this->Mount_Static = new lws_http_mount[1];

	this->Mount_Static->protocol = NULL;
	this->Mount_Static->cgienv = NULL;
	this->Mount_Static->extra_mimetypes = NULL; // We may wish to expose this in future
	this->Mount_Static->interpret = NULL;
	this->Mount_Static->cgi_timeout = 0;
	this->Mount_Static->cache_max_age = 0;
	this->Mount_Static->auth_mask = 0;
	this->Mount_Static->cache_reusable = 0;
	this->Mount_Static->cache_revalidate = 0;
	this->Mount_Static->cache_intermediaries = 0;
	this->Mount_Static->origin_protocol = LWSMPRO_FILE;
	this->Mount_Static->mountpoint_len = Lenght_Mount;
	this->Mount_Static->mountpoint = MountPoint;
	this->Mount_Static->basic_auth_login_file = NULL;
	this->Mount_Static->origin = Origin;
	this->Mount_Static->def = "index.html";

	this->Protocols = new lws_protocols[2];
	this->Protocols[0].name = "http-only";
	this->Protocols[0].callback = lws_callback_http_dummy;
	this->Protocols[0].rx_buffer_size = 10 * 1024 * 1024;
	this->Protocols[0].per_session_data_size = 0;
	this->Protocols[1].name = nullptr;
	this->Protocols[1].callback = nullptr;
	this->Protocols[1].per_session_data_size = 0;

	static struct lws_context_creation_info Info;
	memset(&Info, 0, sizeof(lws_context_creation_info));
	Info.error_document_404 = "/404.html";
	Info.mounts = this->Mount_Static;
	Info.port = this->Port_HTTP;
	Info.protocols = &this->Protocols[0];
	Info.user = (void*)this;
	Info.gid = -1;
	Info.uid = -1;
	Info.options = LWS_SERVER_OPTION_ALLOW_LISTEN_SHARE;

	this->Context = lws_create_context(&Info);
}

void FHTTP_Thread_LibWebSocket::Callback_HTTP_Stop()
{
	if (this->Context)
	{
		lws_context_destroy(this->Context);
		this->Context = NULL;
	}

	this->Protocols = nullptr;
}