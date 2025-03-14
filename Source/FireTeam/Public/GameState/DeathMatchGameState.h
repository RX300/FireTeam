// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameState/OnlineGameState.h"
#include "DeathMatchGameState.generated.h"

/**
 * 
 */
UCLASS()
class FIRETEAM_API ADeathMatchGameState : public AOnlineGameState
{
	GENERATED_BODY()
public:
	virtual void NetMulticast_KillAnnounced_Implementation(APlayerState* playerstate, const FString& Victim)override;
	
};
