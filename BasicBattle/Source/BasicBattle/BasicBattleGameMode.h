// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BasicBattleGameMode.generated.h"

UCLASS(minimalapi)
class ABasicBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABasicBattleGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void OnPlayerDead();
	void OnEnemyDead();


private:

};



