// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Portal/SignIn/SignInOverlay.h"

void APortalHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (IsValid(PlayerController) && SignInOverlayClass)
	{
		SignInOverlay = CreateWidget<USignInOverlay>(PlayerController, SignInOverlayClass);
		SignInOverlay->AddToViewport();
	}
}
