// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BasicBattleGameMode.h"
#include "BasicBattlePlayerController.h"
#include "BasicBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABasicBattleGameMode::ABasicBattleGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABasicBattlePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}