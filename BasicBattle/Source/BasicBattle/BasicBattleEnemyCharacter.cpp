// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicBattleEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABasicBattleEnemyCharacter::ABasicBattleEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicBattleEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicBattleEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicBattleEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicBattleEnemyCharacter::DoAttack_Implementation()
{
	return;
}

TSharedPtr<FCollisionObjectQueryParams> ABasicBattleEnemyCharacter::GetTraceObject(const TArray<ECollisionChannel>& channels)
{
	auto TraceObject = MakeShared<FCollisionObjectQueryParams>();
	for (auto channel : channels)
		TraceObject->AddObjectTypesToQuery(channel);
	return TraceObject;
}

TSharedPtr<FCollisionQueryParams> ABasicBattleEnemyCharacter::GetTraceParams()
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

void ABasicBattleEnemyCharacter::AttackHit()
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

float ABasicBattleEnemyCharacter::GiveDamage(const FHitResult & HitResult)
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