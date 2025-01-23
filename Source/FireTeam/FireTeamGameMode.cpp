// Copyright Epic Games, Inc. All Rights Reserved.

#include "FireTeamGameMode.h"
#include "FireTeamCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFireTeamGameMode::AFireTeamGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_Game/Blueprints/Characters/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
