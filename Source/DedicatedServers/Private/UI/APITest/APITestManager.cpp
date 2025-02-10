// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APITest/APITestManager.h"

void UAPITestManager::OnListFleetClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnListFleetClicked"));
}
