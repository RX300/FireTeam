// Fill out your copyright notice in the Description page of Project Settings.  


#include "GameMode/DeathMatchGameMode.h"  
#include <GameState/OnlineGameState.h>  
#include <Kismet/KismetSystemLibrary.h>  
#include <Kismet/GameplayStatics.h>

void ADeathMatchGameMode::EndMatch()  
{   
    // 打印当前游戏状态
    FString CurrentState;
    //FName MatchStateName = GetMatchState();
    //if (MatchStateName == MatchState::InProgress)
    //{
    //    CurrentState = TEXT("InProgress");
    //}
    //else if (MatchStateName == MatchState::WaitingPostMatch)
    //{
    //    CurrentState = TEXT("WaitingPostMatch");
    //}
    //else if (MatchStateName == MatchState::LeavingMap)
    //{
    //    CurrentState = TEXT("LeavingMap");
    //}
    //else
    //{
    //    CurrentState = TEXT("Other");
    //}

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
    //    FString::Printf(TEXT("ReadyToEndMatch called with state: %s"), *CurrentState));

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("Match Ended"));  
    // 打印当前游戏状态
    auto curState = GetMatchState();
    CurrentState = curState.ToString();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("Current Match State: ") + CurrentState);
}

void ADeathMatchGameMode::CustomSetMatchState(FName NewState)
{
    if (NewState == MatchState::WaitingPostMatch)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ADeathMatchGameMode::BackToLobbyMeny, 5.0f, false);
	}
}

void ADeathMatchGameMode::BackToLobbyMeny()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Back to Lobby Menu"));
	//Get cur PlayerController
	APlayerController* CurPC = UGameplayStatics::GetPlayerController(this, 0);
    FString command(TEXT("ServerTravel /Game/_Game/Maps/Menu/LobbyMenu?Listen?Game=/Game/_Game/Blueprints/Lobby/BP_LobbyGameMode.BP_LobbyGameMode.BP_LobbyGameMode_C"));
    //执行控制台命令
	UKismetSystemLibrary::ExecuteConsoleCommand(this, command, CurPC);
}

bool ADeathMatchGameMode::ReadyToEndMatch_Implementation()  
{  
   //Get GameState  
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
           OnMatchEnd.Broadcast(WinnerID, WinningTeamID);  
           SetMatchState(MatchState::WaitingPostMatch);
           //打印当前时间  
           double TimeInSeconds = UKismetSystemLibrary::GetGameTimeInSeconds(this);  
           FString Time = FString::SanitizeFloat(TimeInSeconds);  
           GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("Match Ended at ") + Time);  
           return true;  
       }  
   }  
   //没有玩家达到胜利条件，不用结束游戏  
   return false;  
}
