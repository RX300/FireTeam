// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/OnlineGameMode.h"
#include "DeathMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIRETEAM_API ADeathMatchGameMode : public AOnlineGameMode
{
	GENERATED_BODY()
public:
	virtual void EndMatch()override;
public:
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void CustomSetMatchState(FName NewState);
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void BackToLobbyMeny();

protected:
	//override ReadyToEndMatch
	virtual bool ReadyToEndMatch_Implementation() override;

	
};
