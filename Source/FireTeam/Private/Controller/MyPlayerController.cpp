// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameMode/OnlineGameMode.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
		Subsystem->AddMappingContext(WeaponInputMappingContext, 1);
	}
	//设置输入模式用于游戏
	SetInputMode(FInputModeGameOnly());
}

void AMyPlayerController::Server_ReSpawnRequest_Implementation()
{
	auto olGameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	olGameMode->RespawnPlayer(this);
}
