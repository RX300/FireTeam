// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MyGameMode.h"

AMyGameMode::AMyGameMode()
	:Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_Game/Blueprints/Characters/BP_PlayerCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game Start"));
}

//void AMyGameMode::StartPlay()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game Start"));
//}
