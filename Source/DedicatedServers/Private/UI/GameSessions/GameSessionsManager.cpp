// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameSessions/GameSessionsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FindOrCreateGameSession_Response") );

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
		DumpMetaData(JsonObject);
 
		FDSGameSession GameSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSession);
		GameSession.Dump();
 
		BroadcastJoinGameSessionMessage.Broadcast(TEXT("找到游戏会话"), false);
	}
}
