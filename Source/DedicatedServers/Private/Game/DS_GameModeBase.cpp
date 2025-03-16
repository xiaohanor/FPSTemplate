// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameModeBase.h"

#include "Player/DSPlayerController.h"

void ADS_GameModeBase::StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	// 绑定定时器结束委托，使用WeakLambda，避免循环引用
	CountdownTimerHandle.TimerFinishedDelegate.BindWeakLambda(this, [&]()
	 {
		 StopCountdownTimer(CountdownTimerHandle);
		 OnCountdownTimerFinished(CountdownTimerHandle.Type);
	 });
	// 倒计时结束后触发定时器结束委托
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle.TimerFinishedHandle,
		CountdownTimerHandle.TimerFinishedDelegate,
		CountdownTimerHandle.CountdownTime,
		false);

	// 绑定定时器更新委托，使用WeakLambda，避免循环引用
	CountdownTimerHandle.TimerUpdateDelegate.BindWeakLambda(this, [&]()
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get());
			if (IsValid(DSPlayerController))
			{
				// 计算剩余时间：总时间 - 已经过去的时间
				const float CountdownTimeLeft = CountdownTimerHandle.CountdownTime - GetWorldTimerManager().GetTimerElapsed(CountdownTimerHandle.TimerFinishedHandle);
				DSPlayerController->Client_TimerUpdated(CountdownTimeLeft, CountdownTimerHandle.Type);
			}
		}
	});
	// 根据 CountdownUpdateInterval 向所有玩家发送定时器更新
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle.TimerUpdateHandle,
		CountdownTimerHandle.TimerUpdateDelegate,
		CountdownTimerHandle.CountdownUpdateInterval,
		true);
}

void ADS_GameModeBase::StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	// 将定时器状态设置为停止
	CountdownTimerHandle.State = ECountdownTimerState::Stopped;
	// 清除定时器
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerFinishedHandle);
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerUpdateHandle);
	// 解绑定时器委托
	if (CountdownTimerHandle.TimerFinishedDelegate.IsBound())
	{
		CountdownTimerHandle.TimerFinishedDelegate.Unbind();
	}
	if (CountdownTimerHandle.TimerUpdateDelegate.IsBound())
	{
		CountdownTimerHandle.TimerUpdateDelegate.Unbind();
	}
	// 通知所有玩家定时器已停止
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get());
		if (IsValid(DSPlayerController))
		{
			DSPlayerController->Client_TimerStopped(0.f, CountdownTimerHandle.Type);
		}
	}
}

void ADS_GameModeBase::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	
}
