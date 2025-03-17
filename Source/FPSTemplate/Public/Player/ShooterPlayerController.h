﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DSPlayerController.h"
#include "ShooterPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStateReplicated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuitMenuOpen, bool, bOpen);

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AShooterPlayerController : public ADSPlayerController
{
	GENERATED_BODY()
public:
	AShooterPlayerController();
	bool bPawnAlive;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;
	virtual void EnableInput(class APlayerController* PlayerController) override;
	virtual void DisableInput(class APlayerController* PlayerController) override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateReplicated OnPlayerStateReplicated;

	UPROPERTY(BlueprintAssignable)
	FOnQuitMenuOpen OnQuitMenuOpen;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> ShooterIMC;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;	

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> QuitAction;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Crouch();
	void Input_Jump();
	void Input_Quit();
	
	bool bQuitMenuOpen;
	
	

	
};
