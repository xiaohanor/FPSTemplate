﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SingIn/SignInOverlay.h"

#include "UI/Portal/PortalManager.h"


void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(PortalManagerClass);
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	
}
