﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalManager.h"

#include "Kismet/KismetSystemLibrary.h"

void UPortalManager::SignIn(const FString& Username, const FString& Password)
{
}

void UPortalManager::SignUp(const FString& Username, const FString& Password, const FString& Email)
{
}

void UPortalManager::Confirm(const FString& ConfirmationCode)
{
}

void UPortalManager::QuitGame()
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		UKismetSystemLibrary::QuitGame(this, LocalPlayerController, EQuitPreference::Quit, false);
	}
}
