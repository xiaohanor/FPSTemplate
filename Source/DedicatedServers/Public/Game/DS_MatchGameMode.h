// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "DS_MatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()
public:
	ADS_MatchGameMode();
	// 启用无缝旅行后将执行 InitSeamlessTravelPlayer 而不是 PostLogin
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
 
	UPROPERTY()
	EMatchStatus MatchStatus;
 
protected:
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	virtual void Logout(AController* Exiting) override;
	void SetClientInputEnabled(bool bEnabled);
	void OnMatchEnded();
 
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimer;
 
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimer;
 
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;
};
