// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BasicBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/DamageType.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

ABasicBattleCharacter::ABasicBattleCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ABasicBattleCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicBattleCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
	
}

void ABasicBattleCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABasicBattleCharacter::PossessedBy(AController * NewController)
{
	UClass *AnimInstanceClass = AnimAssetClass.TryLoadClass<UAnimInstance>();
	if (AnimInstanceClass)
	{
		UE_LOG(LogClass, Warning, TEXT("Loading Character Anim Asset.. "));
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	}
}

float ABasicBattleCharacter::TakeDamage(float Damage, const FDamageEvent & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	/*
	if (HOFPlayerState->PlayerData.HP.CheckOnMinValue())
	{
		auto gameMode = Cast<AHOFBattleGameMode>(GetWorld()->GetAuthGameMode());
		gameMode->OnPlayerDead();
		return 0.f;
	}
	*/
	if (ActualDamage > 0.f)
	{
		//HOFPlayerState->PlayerData.HP(-ActualDamage);
		//AB_LOG(Warning, TEXT("HP:%f"), HOFPlayerState->CurrentHP);

		//if (HOFPlayerState->PlayerData.HP.CheckOnMinValue())
			//HOFPlayerState->SetState(EHOFCharacterState::PLAYER_DEAD);
	}
	return ActualDamage;
}

void ABasicBattleCharacter::AttackHit()
{
	FHitResult HitResult(ForceInit);
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + GetActorForwardVector() * 100.0f;
	auto TraceParams = GetTraceParams();
	auto TraceObject = GetTraceObject(TArray<ECollisionChannel>{ECC_Pawn, ECC_WorldStatic});
	
	//if (GetWorld()->LineTraceSingleByObjectType(HitResult, StartPos, EndPos, *TraceObject, *TraceParams))
	if (GetWorld()->SweepSingleByObjectType(HitResult, StartPos, EndPos, FQuat(), *TraceObject, FCollisionShape::MakeSphere(50.0f), *TraceParams))
		GiveDamage(HitResult);
		
}

TSharedPtr<FCollisionObjectQueryParams> ABasicBattleCharacter::GetTraceObject(const TArray<ECollisionChannel>& channels)
{
	auto TraceObject = MakeShared<FCollisionObjectQueryParams>();
	for (auto channel : channels)
		TraceObject->AddObjectTypesToQuery(channel);
	return TraceObject;
}

TSharedPtr<FCollisionQueryParams> ABasicBattleCharacter::GetTraceParams()
{
	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;

	auto TraceParams = MakeShared<FCollisionQueryParams>(FName(TEXT("VictoreCore Trace")), true, this);
	TraceParams->bTraceComplex = true;
	TraceParams->bReturnPhysicalMaterial = false;
	TraceParams->TraceTag = TraceTag;

	//Ignore Actors
	TraceParams->AddIgnoredActor(this);
	return TraceParams;
}

float ABasicBattleCharacter::GiveDamage(const FHitResult & HitResult)
{
	float BaseDamage = 30.0f;
	float WeaponDamage = 0.0f;

	/*
	// Get Attack Info from Current Weapon
	const auto& SlotItem = Equip->GetEquippedItemBySlot(EHOFItemType::ITEM_MAIN_WEAPON);


	do
	{
		if (!SlotItem)
			break;

		if (!SlotItem->IsWeaponItem())
			break;

		AHOFWeaponItem* Weapon = static_cast<AHOFWeaponItem*>(SlotItem);

		WeaponDamage += Weapon->GetAttackDamage();
	} while (0);
	*/

	if (HitResult.GetComponent()->GetFName() == FName(TEXT("HeadCollision")))
	{
		BaseDamage += BaseDamage;
	}

	float FinalDamage = BaseDamage + WeaponDamage;
	FPointDamageEvent PointDamageEvent(FinalDamage, HitResult, GetActorForwardVector(), UDamageType::StaticClass());
	float ResultDamage = HitResult.GetActor()->TakeDamage(FinalDamage, PointDamageEvent, GetController(), this);

	return ResultDamage;
}
