// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicBattleCharacter.generated.h"

UCLASS(Blueprintable)
class ABasicBattleCharacter : public ACharacter
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
	
	UPROPERTY(config, EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
		float MaxHP;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stat")
		float CurrentHP;

	UPROPERTY(config)
		TArray<FStringAssetReference>CharacterAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
		bool isAttacking;

	UPROPERTY(config)
		FStringClassReference AnimAssetClass;

private:
	TSharedPtr<FCollisionObjectQueryParams> GetTraceObject(const TArray<ECollisionChannel>& channels);
	TSharedPtr<FCollisionQueryParams> GetTraceParams();
	void GiveDamage(const FHitResult & HitResult);

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
};

