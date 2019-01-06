// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicBattlePlayerController.generated.h"

class ABasicBattleCharacter;

UCLASS()
class ABasicBattlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasicBattlePlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	
private:
	ABasicBattleCharacter * PlayerCharacter;

	float CurrentLeftRightVal;
	float CurrentUpDownVal;

	UFUNCTION()
		void MoveForward(float NewInputVal);

	UFUNCTION()
		void MoveRight(float NewInputVal);

	UFUNCTION()
		void Turn(float NewInputVal);

	UFUNCTION()
		void LookUp(float NewInputVal);

	UFUNCTION()
		void JumpInput();

	UFUNCTION()
		void JumpIfNotInput();

	UFUNCTION()
		void Attack();

	UFUNCTION()
		void NotAttack();

	UFUNCTION()
		void Slot1();

	UFUNCTION()
		void Slot2();

	UFUNCTION()
		void SetStateToBattle();
};


