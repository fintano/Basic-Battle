// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "BasicBattleCharacter.generated.h"


//Example for an enum the FGameplayAbiliyInputBinds may use to map input to ability slots.

//It's very important that this enum is UENUM, because the code will look for UENUM by the given name and crash if the UENUM can't be found. BlueprintType is there so we can use these in blueprints, too. Just in case. Can be neat to define ability packages.
UENUM(BlueprintType)
enum class AbilityInput : uint8
{
	UseAbility1 UMETA(DisplayName = "Use Spell 1"), //This maps the first ability(input ID should be 0 in int) to the action mapping(which you define in the project settings) by the name of "UseAbility1". "Use Spell 1" is the blueprint name of the element.
	UseAbility2 UMETA(DisplayName = "Use Spell 2"), //Maps ability 2(input ID 1) to action mapping UseAbility2. "Use Spell 2" is mostly used for when the enum is a blueprint variable.
	UseAbility3 UMETA(DisplayName = "GA TheSymbolOfEmpire"),
	UseAbility4 UMETA(DisplayName = "Use Spell 4"),
	WeaponAbility UMETA(DisplayName = "Use Weapon"), //This finally maps the fifth ability(here designated to be your weaponability, or auto-attack, or whatever) to action mapping "WeaponAbility".

		//You may also do something like define an enum element name that is not actually mapped to an input, for example if you have a passive ability that isn't supposed to have an input. This isn't usually necessary though as you usually grant abilities via input ID,
		//which can be negative while enums cannot. In fact, a constant called "INDEX_NONE" exists for the exact purpose of rendering an input as unavailable, and it's simply defined as -1.
		//Because abilities are granted by input ID, which is an int, you may use enum elements to describe the ID anyway however, because enums are fancily dressed up ints.
};

class UBasicBattleAttributeSet;

UCLASS(Blueprintable)
class ABasicBattleCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABasicBattleCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(class AController *NewController) override;
	virtual float TakeDamage(float Damage, const struct FDamageEvent &DamageEvent, class AController* EventInstigator, class AActor*DamageCauser) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UFUNCTION(BlueprintCallable, Category = "Damage")
		void AttackHit();

	UFUNCTION(BlueprintCallable, Category = "Damage")
		TArray<AActor*> AttackRadialHit();

	UFUNCTION(BlueprintCallable, Category = "Damage")
		float GiveDamage(const FHitResult & HitResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Animation")
		void PlayHitAction();
	
	UCameraComponent* GetCamera() {	return TopDownCameraComponent; }

public : 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Collision")
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Visible")
		class USkeletalMeshComponent* Body;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Visible")
		class USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Visible")
		class UArrowComponent* Arrow;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		class UFloatingPawnMovement*FloatPawnMovement;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Camera")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent* AbilitySystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TArray<TSubclassOf<class UGameplayAbility>> GamePlayAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBasicBattleAttributeSet* AttributeSet;

	UPROPERTY(config, EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
		float MaxHP;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stat")
		float CurrentHP;

	UPROPERTY(config)
		TArray<FStringAssetReference>CharacterAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool isAlive;

	UPROPERTY(config)
		FStringClassReference AnimAssetClass;

private:
	TSharedPtr<FCollisionObjectQueryParams> GetTraceObject(const TArray<ECollisionChannel>& channels);
	TSharedPtr<FCollisionQueryParams> GetTraceParams();

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	// Inherited via IAbilitySystemInterface
	virtual UAbilitySystemComponent * GetAbilitySystemComponent() const override;
};

