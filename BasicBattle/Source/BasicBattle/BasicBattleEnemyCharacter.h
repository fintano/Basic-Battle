// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicBattleEnemyCharacter.generated.h"

UCLASS()
class BASICBATTLE_API ABasicBattleEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicBattleEnemyCharacter();

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void DoAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
		void AttackHit();

	UFUNCTION(BlueprintCallable, Category = "Damage")
		float GiveDamage(const FHitResult & HitResult);

private:
	TSharedPtr<FCollisionObjectQueryParams> GetTraceObject(const TArray<ECollisionChannel>& channels);
	TSharedPtr<FCollisionQueryParams> GetTraceParams();

public : 

	UPROPERTY(config, EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
		float MaxHP;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stat")
		float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool isAlive;
};
