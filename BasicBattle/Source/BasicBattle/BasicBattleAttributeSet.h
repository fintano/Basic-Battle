// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BasicBattleAttributeSet.generated.h"

/**
 *
 */
UCLASS()
class BASICBATTLE_API UBasicBattleAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBasicBattleAttributeSet();
	UPROPERTY(Category = "Attribute | Health", EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Health;

	static FGameplayAttribute AttributeHealth();

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
