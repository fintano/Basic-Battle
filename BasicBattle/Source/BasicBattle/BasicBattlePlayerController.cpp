// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BasicBattlePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "BasicBattleCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ABasicBattlePlayerController::ABasicBattlePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ABasicBattlePlayerController::BeginPlay()
{
	PlayerCharacter = Cast<ABasicBattleCharacter>(GetCharacter());
	if (PlayerCharacter == NULL)
	{
		// ¾î¶±ÇÏÁö
	}
}

void ABasicBattlePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void ABasicBattlePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ABasicBattlePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ABasicBattlePlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ABasicBattlePlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ABasicBattlePlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ABasicBattlePlayerController::OnResetVR);

	Super::SetupInputComponent();
	//InputComponent->BindAxis("MoveRight", this, &AABPlayerController::LeftRightInput);
	//InputComponent->BindAxis("MoveForward", this, &AABPlayerController::UpDownInput);

	InputComponent->BindAxis("MoveForward", this, &ABasicBattlePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABasicBattlePlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ABasicBattlePlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ABasicBattlePlayerController::LookUp);

	// TEST 
	InputComponent->BindAction("Slot1", IE_Pressed, this, &ABasicBattlePlayerController::Slot1);
	InputComponent->BindAction("Slot2", IE_Pressed, this, &ABasicBattlePlayerController::Slot2);
	//
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABasicBattlePlayerController::JumpInput);
	InputComponent->BindAction("Jump", IE_Released, this, &ABasicBattlePlayerController::JumpIfNotInput);
	InputComponent->BindAction("Attack", IE_Pressed, this, &ABasicBattlePlayerController::Attack);
	InputComponent->BindAction("Attack", IE_Released, this, &ABasicBattlePlayerController::NotAttack);
}

void ABasicBattlePlayerController::MoveForward(float NewInputVal)
{
	if (NewInputVal != 0.0f)
	{
		// find out which way is forward
		FRotator Rotation = PlayerCharacter->GetCamera()->GetComponentRotation();
		// Limit pitch when walking or falling
		if (PlayerCharacter->GetCharacterMovement()->IsMovingOnGround() || PlayerCharacter->GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		PlayerCharacter->AddMovementInput(Direction, NewInputVal);

		const FRotator curRotation = PlayerCharacter->GetActorRotation();
		const FVector XScaledCurRotation = FRotationMatrix(curRotation).GetScaledAxis(EAxis::X);
		FRotator newRotation = UKismetMathLibrary::MakeRotFromX(FVector(NewInputVal, XScaledCurRotation.Y, 0));
		SetControlRotation(newRotation);
	}
}

void ABasicBattlePlayerController::MoveRight(float NewInputVal)
{
	if (NewInputVal != 0.0f)
	{
		FRotator rotation = PlayerCharacter->GetCamera()->GetComponentRotation(); //Controller->GetControlRotation();
		const FVector direction = FRotationMatrix(rotation).GetScaledAxis(EAxis::Y);
		PlayerCharacter->AddMovementInput(direction, NewInputVal);

		const FRotator curRotation = PlayerCharacter->GetActorRotation();
		const FVector XScaledCurRotation = FRotationMatrix(curRotation).GetScaledAxis(EAxis::X);
		FRotator newRotation = UKismetMathLibrary::MakeRotFromX(FVector(XScaledCurRotation.X, NewInputVal, 0));
		SetControlRotation(newRotation);
	}
}

void ABasicBattlePlayerController::Turn(float NewInputVal)
{
	PlayerCharacter->AddControllerYawInput(NewInputVal);
}

void ABasicBattlePlayerController::LookUp(float NewInputVal)
{
	PlayerCharacter->AddControllerPitchInput(NewInputVal);
}

void ABasicBattlePlayerController::JumpInput()
{
	PlayerCharacter->bPressedJump = true;
}

void ABasicBattlePlayerController::JumpIfNotInput()
{
	PlayerCharacter->bPressedJump = false;
}

void ABasicBattlePlayerController::Attack()
{
	SetStateToBattle();
	PlayerCharacter->isAttacking = true;
}

void ABasicBattlePlayerController::NotAttack()
{
}

void ABasicBattlePlayerController::Slot1()
{
}

void ABasicBattlePlayerController::Slot2()
{
}

void ABasicBattlePlayerController::SetStateToBattle()
{
	/*
	AHOFPlayerState *playerState = Cast<AHOFPlayerState>(PlayerState);
	if (playerState)
	{
		if (playerState->bIsBattleInAction)
			return;

		playerState->SetState(EHOFCharacterState::PLAYER_BATTLE);
	}
	*/
}

void ABasicBattlePlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABasicBattlePlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ABasicBattleCharacter* MyPawn = Cast<ABasicBattleCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ABasicBattlePlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ABasicBattlePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ABasicBattlePlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ABasicBattlePlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
