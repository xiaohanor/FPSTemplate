// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "DSLocalPlayerSubsystem.generated.h"

class UPortalManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDSLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	void InitializeTokens(const FDSAuthenticationResult& AuthResult, UPortalManager* InPortalManager);
	void RefreshTokenTimer();
	void UpdatedTokens(const FString& AccessToken, const FString& IdToken);
private:
	UPROPERTY()
	FDSAuthenticationResult AuthenticationResult;

	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	// Token刷新间隔
	float TokenRefreshInterval = 1.0f;
	FTimerHandle TokenRefreshTimer;
};
