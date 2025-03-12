// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/OnlineGameMode.h"

void AOnlineGameMode::RespawnPlayer(AController* Controller)
{
	//»ñÈ¡Pawn
	APawn* Pawn = Controller->GetPawn();
	//Ïú»ÙActor
	bool valid = Pawn->Destroy();
	//respawn player
	if (valid)
	{
		RestartPlayer(Controller);
	}
}
