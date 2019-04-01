// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicBattleAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UBasicBattleAttributeSet::UBasicBattleAttributeSet() : Super(), Health(100.f)
{

}

bool UBasicBattleAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data)
{
	if (AttributeHealth() == Data.EvaluatedData.Attribute)
		UE_LOG(LogClass, Display, TEXT("Attribute Health : %f"), Health.GetCurrentValue());
	return true;
}

void UBasicBattleAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (AttributeHealth() == Data.EvaluatedData.Attribute)
		UE_LOG(LogClass, Display, TEXT("Attribute Health : %f"), Health.GetCurrentValue());

	//if (AttributeHealth() == Data.EvaluatedData.Attribute)
	//{
	//	if (Health.GetCurrentValue() < 0.f)
	//	{
	//		OnDeath.Broadcast(Data.EvaluatedData, Data.EffectSpec);
	//	}
	//}
}

void UBasicBattleAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

}


FGameplayAttribute UBasicBattleAttributeSet::AttributeHealth()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBasicBattleAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBasicBattleAttributeSet, Health));
	return FGameplayAttribute(Property);
}