// Copyright Epic Games, Inc. All Rights Reserved.

#include "BrackeysGameJam2021GameMode.h"
#include "BrackeysGameJam2021Character.h"
#include "UObject/ConstructorHelpers.h"

ABrackeysGameJam2021GameMode::ABrackeysGameJam2021GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
