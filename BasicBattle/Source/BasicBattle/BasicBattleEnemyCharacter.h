// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BasicBattleEnemyCharacter.generated.h"

class UBasicBattleAttributeSet;

UENUM(BlueprintType)
enum EAction
{
	ACTION_ATTACK UMETA(DisplayName = "ACTION_ATTACK"),
	ACTION_SKILL_HASTE UMETA(DisplayName = "ACTION_SKILL_HASTE"),
	ACTION_SKILL_SPECIAL_ATTACK UMETA(DisplayName = "ACTION_SKILL_SPECIAL_ATTACK"),
};

UCLASS()
class BASICBATTLE_API ABasicBattleEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicBattleEnemyCharacter();
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void DoAttack();

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void DoSkill(EAction action);

	UFUNCTION(BlueprintCallable, Category = "Action")
		void DoAction(EAction action);

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
	// Inherited via IAbilitySystemInterface
	virtual UAbilitySystemComponent * GetAbilitySystemComponent() const override;
	void CheckAttackAction(EAction action);
	void CheckSkillAction(EAction action);

public : 

	UPROPERTY(config, EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
		float MaxHP;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stat")
		float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool isAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBasicBattleAttributeSet* AttributeSet;

private : 
	float haste = 1.0f;
	float lastAttackTime = 0.0f;
	float attackCooldown = 1.0f;
	float lastSkill1Time = 0.0f;
	float skill1Cooldown = 12.0f;
	float lastSkill2Time = 0.0f;
	float skill2Cooldown = 5.0f;
};
