﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Types/DSTypes.h"
#include "DSPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimerStateChangedDelegate, float, Time, ECountdownTimerType, Type);

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADSPlayerController();
	virtual void ReceivedPlayer() override;

	UFUNCTION(Client, Reliable)
	void Client_TimerUpdated(float CountdownTimeLeft, ECountdownTimerType Type) const;
 
	UFUNCTION(Client, Reliable)
	void Client_TimerStopped(float CountdownTimeLeft, ECountdownTimerType Type) const;
 
	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangedDelegate OnTimerUpdated;
 
	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangedDelegate OnTimerStopped;
	
protected:
	UFUNCTION(Server,Reliable)
	void ServerPing(float TimeOfRequest);

	UFUNCTION(Client,Reliable)
	void ClientPong(float TimeOfRequest);
private:
	float SingleTripTime;
};
