// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/OnlineGameMode.h"
#include "GameState/OnlineGameState.h"
void AOnlineGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//Get GameState
	AOnlineGameState* CurGameState = GetGameState<AOnlineGameState>();
	if (CurGameState)
	{
		//Call Server_NewPlayerJoined
		CurGameState->NewPlayerJoined(NewPlayer);
	}
}

void AOnlineGameMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	//Get GameState
	AOnlineGameState* CurGameState = GetGameState<AOnlineGameState>();
	if (CurGameState)
	{
		//Call Server_NewPlayerJoined
	}	CurGameState->NewPlayerJoined(NewPC);
}

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
