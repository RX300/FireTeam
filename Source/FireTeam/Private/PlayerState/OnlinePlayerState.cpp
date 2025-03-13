// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/OnlinePlayerState.h"
#include "Net/UnrealNetwork.h"
void AOnlinePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOnlinePlayerState, PersonalScore);
	DOREPLIFETIME(AOnlinePlayerState, Kills);
	DOREPLIFETIME(AOnlinePlayerState, Deaths);
	DOREPLIFETIME(AOnlinePlayerState, TeamID);
}

void AOnlinePlayerState::GainKills(const FString& Victim)
{
	Kills++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Kills: %d"), Kills));
	OnKillEarned.Broadcast(this, Victim);
}
