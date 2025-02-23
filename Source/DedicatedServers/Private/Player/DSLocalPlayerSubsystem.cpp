// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSLocalPlayerSubsystem.h"

#include "UI/Portal/PortalManager.h"

void UDSLocalPlayerSubsystem::InitializeTokens(const FDSAuthenticationResult& AuthResult,
                                               UPortalManager* InPortalManager)
{
	AuthenticationResult = AuthResult;
	PortalManager = InPortalManager;
	RefreshTokenTimer();
}

void UDSLocalPlayerSubsystem::RefreshTokenTimer()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		FTimerDelegate RefreshDelegate;
		RefreshDelegate.BindLambda([this]{PortalManager->RefreshTokens(AuthenticationResult.RefreshToken);});
		World->GetTimerManager().SetTimer(TokenRefreshTimer, RefreshDelegate, TokenRefreshInterval, false);
	}
}

void UDSLocalPlayerSubsystem::UpdatedTokens(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	RefreshTokenTimer();
}
