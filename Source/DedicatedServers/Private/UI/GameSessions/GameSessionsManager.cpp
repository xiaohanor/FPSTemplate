// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameSessions/GameSessionsManager.h"

void UGameSessionsManager::JoinGameSession()
{
	BroadcastJoinGameSession.Broadcast(TEXT("搜索游戏会话中..."), false);
}
