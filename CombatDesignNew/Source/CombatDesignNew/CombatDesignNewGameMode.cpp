// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatDesignNewGameMode.h"
#include "CombatDesignNewCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACombatDesignNewGameMode::ACombatDesignNewGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/BP_PlayerCharacter.BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
