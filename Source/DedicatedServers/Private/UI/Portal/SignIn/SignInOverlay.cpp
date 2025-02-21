// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "UI/API/GameSessions/JoinGame.h"
#include "UI/Portal/PortalManager.h"


void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(PortalManagerClass);
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &USignInOverlay::OnJoinGameButtonClicked);

	check(Button_SignIn_Test);
	check(Button_SignUp_Test);
	check(Button_ConfirmSignUp_Test);
	check(Button_SuccessConfirmed_Test);

	Button_SignIn_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	Button_SignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	Button_ConfirmSignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowConfirmSignUpPage);
	Button_SuccessConfirmed_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSuccessConfirmedPage);
}

void USignInOverlay::OnJoinGameButtonClicked()
{
	check(IsValid(PortalManager));

	PortalManager->BroadcastJoinGameSession.AddDynamic(this, &USignInOverlay::UpdateJoinGameStatusMessage);
	PortalManager->JoinGameSession();
	JoinGameWidget->Button_JoinGame->SetIsEnabled(false);
}

void USignInOverlay::UpdateJoinGameStatusMessage(const FString& StatusMessage)
{
	JoinGameWidget->SetStatusMessage(StatusMessage);
}

void USignInOverlay::ShowSignInPage()
{
}

void USignInOverlay::ShowSignUpPage()
{
}

void USignInOverlay::ShowConfirmSignUpPage()
{
}

void USignInOverlay::ShowSuccessConfirmedPage()
{
}
