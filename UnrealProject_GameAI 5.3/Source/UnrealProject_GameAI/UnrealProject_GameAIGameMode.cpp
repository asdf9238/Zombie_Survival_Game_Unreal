// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealProject_GameAIGameMode.h"
#include "UnrealProject_GameAICharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealProject_GameAIGameMode::AUnrealProject_GameAIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
