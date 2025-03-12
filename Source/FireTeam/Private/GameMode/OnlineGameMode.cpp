// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/OnlineGameMode.h"

void AOnlineGameMode::RespawnPlayer(AController* Controller)
{
	//��ȡPawn
	APawn* Pawn = Controller->GetPawn();
	//����Actor
	bool valid = Pawn->Destroy();
	//respawn player
	if (valid)
	{
		RestartPlayer(Controller);
	}
}
