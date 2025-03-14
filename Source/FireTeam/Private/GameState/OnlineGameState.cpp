// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/OnlineGameState.h"
#include "UI/KillAnnouncementWidget.h"
#include "UI/KillAnnouncementTrayWidget.h"
#include "UI/InGameHUD.h"
#include "GameFramework/PlayerState.h" // ��Ӵ����Խ�����������ʹ���
#include "PlayerState/OnlinePlayerState.h"


void AOnlineGameState::Server_AnnounceKill_Implementation(APlayerState* playerstate, const FString& Victim)
{
	NetMulticast_KillAnnounced(playerstate, Victim);
}

void AOnlineGameState::NetMulticast_KillAnnounced_Implementation(APlayerState* playerstate, const FString& Victim)
{
	//����KillAnnouncementWidget��UI�ؼ�
	TObjectPtr<UKillAnnouncementWidget> KillAnnouncementWidget = CreateWidget<UKillAnnouncementWidget>(GetWorld(), KillAnnouncementWidgetClass);
	KillAnnouncementWidget->KillerName = FText::FromString(playerstate->GetPlayerName());
	KillAnnouncementWidget->VictimName = FText::FromString(Victim);
	KillAnnouncementWidget->Init();
	//ͨ����ҿ�������ȡ��HUD����ת����InGameHUD��ȡ��KillAnnouncementTrayWidget����KillAnnouncementWidget��ӵ�KillAnnouncementTrayWidget��
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		AInGameHUD* InGameHUD = Cast<AInGameHUD>(PlayerController->GetHUD());
		if (InGameHUD)
		{
			UKillAnnouncementTrayWidget* KillAnnouncementTrayWidget = Cast<UKillAnnouncementTrayWidget>(InGameHUD->InGameHUDWidget->GetWidgetFromName("KillAnnouncementTray"));
			if (KillAnnouncementTrayWidget)
			{
				KillAnnouncementTrayWidget->AddAnnouncement(KillAnnouncementWidget);
			}
		}
	}
}

//��GameMode�����
void AOnlineGameState::NewPlayerJoined(APlayerController* NewPlayer)
{
	//Get PlayerState
	APlayerState* PlayerState = NewPlayer->PlayerState;
	//Cast To OnlinePlayerState
	AOnlinePlayerState* OnlinePlayerState = Cast<AOnlinePlayerState>(PlayerState);
	//Bind Event To OnKillEarned
	OnlinePlayerState->OnKillEarned.AddDynamic(this, &AOnlineGameState::Server_AnnounceKill);
}

void AOnlineGameState::AddPlayerPoint(int PlayerID, int Score)
{
	PlayerScoreBoard.FindOrAdd(PlayerID) += Score;
	FScoreData ScoreData;
	ScoreData.PlayerScores = PlayerScoreBoard;
	ScoreData.TeamScores = TeamScoreBoard;
	OnScoreUpdated.Broadcast(ScoreData);
}

void AOnlineGameState::AddTeamPoint(int TeamID, int Score)
{
	TeamScoreBoard.FindOrAdd(TeamID) += Score;
	FScoreData ScoreData;
	ScoreData.PlayerScores = PlayerScoreBoard;
	ScoreData.TeamScores = TeamScoreBoard;
	OnScoreUpdated.Broadcast(ScoreData);
}
