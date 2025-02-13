// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalManager.h"

void UPortalManager::JoinGameSession()
{
	BroadcastJoinGameSession.Broadcast(TEXT("搜索游戏会话中..."));
}
