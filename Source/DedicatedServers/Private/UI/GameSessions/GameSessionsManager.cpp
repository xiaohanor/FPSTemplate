﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameSessions/GameSessionsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/APIData.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UGameSessionsManager::JoinGameSession()
{
	check(APIData);
	BroadcastJoinGameSessionMessage.Broadcast(TEXT("搜索游戏会话中..."), false);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::FindOrCreateGameSession_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::FindOrCreateGameSession);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		Request->SetHeader(TEXT("Authorization"), LocalPlayerSubsystem->GetAuthResult().AccessToken);
	}
	
	Request->ProcessRequest();
}

void UGameSessionsManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}
 
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		}
 
		FDSGameSession GameSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSession);
		
		const FString GameSessionId = GameSession.GameSessionId;
		const FString GameSessionStatus = GameSession.Status;
		HandleGameSessionStatus(GameSessionStatus, GameSessionId);
	}
}

FString UGameSessionsManager::GetUniquePlayerId() const
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		APlayerState* LocalPlayerState = LocalPlayerController->GetPlayerState<APlayerState>();
		if (IsValid(LocalPlayerState) && LocalPlayerState->GetUniqueId().IsValid())
		{
			return TEXT("Player_") + FString::FromInt(LocalPlayerState->GetUniqueID());
		}
	}
	return FString();
}

void UGameSessionsManager::HandleGameSessionStatus(const FString& Status, const FString& SessionId)
{
	if (Status.Equals(TEXT("ACTIVE")))
	{
		BroadcastJoinGameSessionMessage.Broadcast(TEXT("正在加入游戏会话..."), false);
		TryCreatePlayerSession(GetUniquePlayerId(), SessionId);
	}
	else if (Status.Equals(TEXT("ACTIVATING")))
	{
		FTimerDelegate CreateSessionDelegate;
		CreateSessionDelegate.BindUObject(this, &ThisClass::JoinGameSession);
		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			LocalPlayerController->GetWorldTimerManager().SetTimer(CreateSessionTimer, CreateSessionDelegate, 0.5f, false);
		}
	}
	else
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}
}

void UGameSessionsManager::TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionId)
{
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::CreatePlayerSession_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::CreatePlayerSession);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
 
	TMap<FString, FString> Params = {
		{ TEXT("playerId"), PlayerId },
		{ TEXT("gameSessionId"), GameSessionId }
	};
	const FString Content = SerializeJsonContent(Params);
 
	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

void UGameSessionsManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}
 
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		}
 
		FDSPlayerSession PlayerSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PlayerSession);

		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			FInputModeGameOnly InputModeData;
			LocalPlayerController->SetInputMode(InputModeData);
			LocalPlayerController->SetShowMouseCursor(false);
		}
		
		const FString IpAndPort = PlayerSession.IpAddress + TEXT(":") + FString::FromInt(PlayerSession.Port);
		const FName Address(*IpAndPort);
		UGameplayStatics::OpenLevel(this, Address);
	}
}
