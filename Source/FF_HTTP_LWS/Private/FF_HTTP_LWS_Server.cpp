// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_HTTP_LWS_Server.h"

// Sets default values
AHTTP_Server_LWS::AHTTP_Server_LWS()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHTTP_Server_LWS::BeginPlay()
{
#ifdef _WIN64
	this->HTTP_Server_Start();
#endif
	
	Super::BeginPlay();
}

void AHTTP_Server_LWS::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
#ifdef _WIN64
	this->HTTP_Server_Stop();
#endif

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AHTTP_Server_LWS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AHTTP_Server_LWS::HTTP_Server_Start()
{
#ifdef _WIN64
	
	if (this->Server_Name.IsEmpty())
	{
		return false;
	}

	this->Thread_LibWebSocket = new FHTTP_Thread_LibWebSocket(this);

	if (this->Thread_LibWebSocket)
	{
		return true;
	}

	else
	{
		return false;
	}

#else

	return false;

#endif
}

void AHTTP_Server_LWS::HTTP_Server_Stop()
{
#ifdef _WIN64
	
	if (this->Thread_LibWebSocket)
	{
		delete this->Thread_LibWebSocket;
		return;
	}

	else
	{
		return;
	}

#else

#endif
}

bool AHTTP_Server_LWS::HTTP_Server_Toggle(bool bIsPause)
{
	if (!this->Thread_LibWebSocket)
	{
		return false;
	}

	return this->Thread_LibWebSocket->Toggle(bIsPause);
}