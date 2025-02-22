// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "GameSessionsManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBroadcastJoinGameSession, const FString&, StatusMessage);

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UGameSessionsManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FBroadcastJoinGameSession BroadcastJoinGameSession;

	void JoinGameSession();
};
