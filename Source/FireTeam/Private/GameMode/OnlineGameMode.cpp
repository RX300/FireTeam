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

bool AOnlineGameMode::ReadyToEndMatch()
{
	//Get GameState
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("ReadyToEndMatch"));
	AOnlineGameState* CurGameState = GetGameState<AOnlineGameState>();
	auto playerScoreBoard = CurGameState->PlayerScoreBoard;
	auto teamScoreBoard = CurGameState->TeamScoreBoard;
	auto ScoreBoard = isTeamType ? teamScoreBoard : playerScoreBoard;
	//foreach ScoreBoard, check if any player's score is greater than WinThreshold
	for (auto& Elem : ScoreBoard)
	{
		if (Elem.Value >= WinThreshold)
		{
			//Set WinnerID
			WinnerID = isTeamType ? -1 : Elem.Key;
			WinningTeamID = isTeamType ? Elem.Key : -1;
			return true;
		}
	}
	//û����Ҵﵽʤ�����������ý�����Ϸ
	return false;
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
