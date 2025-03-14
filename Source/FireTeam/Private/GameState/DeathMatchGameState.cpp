// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/DeathMatchGameState.h"
#include "PlayerState/OnlinePlayerState.h"
#include "UI/KillAnnouncementWidget.h"
#include "UI/KillAnnouncementTrayWidget.h"
#include "UI/InGameHUD.h"
void ADeathMatchGameState::NetMulticast_KillAnnounced_Implementation(APlayerState* playerstate, const FString& Victim)
{
	Super::NetMulticast_KillAnnounced_Implementation(playerstate, Victim);
    // ÃÌº”ø’÷∏’ÎºÏ≤È
    AOnlinePlayerState* OnlinePlayerState = Cast<AOnlinePlayerState>(playerstate);
    if (OnlinePlayerState)
    {
        OnlinePlayerState->GainScore(1);
    }
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerState is not Getted"));
	}
}
