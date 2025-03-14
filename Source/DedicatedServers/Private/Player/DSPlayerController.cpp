// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSPlayerController.h"

ADSPlayerController::ADSPlayerController()
{
	SingleTripTime = 0.f;
}

void ADSPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	// 仅在本地且联机模式上运行
	if (GetNetMode() == NM_Standalone) return;
	if (IsLocalController())
	{
		ServerPing(GetWorld()->GetTimeSeconds());
	}
}

void ADSPlayerController::Client_TimerUpdated_Implementation(float CountdownTimeLeft, ECountdownTimerType Type) const
{
	OnTimerUpdated.Broadcast(CountdownTimeLeft - SingleTripTime, Type);
}

void ADSPlayerController::Client_TimerStopped_Implementation(float CountdownTimeLeft, ECountdownTimerType Type) const
{
	OnTimerStopped.Broadcast(CountdownTimeLeft - SingleTripTime, Type);
}

void ADSPlayerController::ServerPing_Implementation(float TimeOfRequest)
{
	ClientPong(TimeOfRequest);
}

void ADSPlayerController::ClientPong_Implementation(float TimeOfRequest)
{
	// 计算单程时间
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfRequest;
	SingleTripTime = RoundTripTime * 0.5;
}

