// Copyright Epic Games, Inc. All Rights Reserved.

#include "FornwestGameMode.h"
#include "FornwestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFornwestGameMode::AFornwestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/Character_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
