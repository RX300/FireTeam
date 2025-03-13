// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OnlinePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnKillEarnedDispatcher,
	APlayerState*, PlayerState,
	const FString&,Victim);
	

/**
 * 
 */
UCLASS()
class FIRETEAM_API AOnlinePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	//GetLifetimeReplicatedProps
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UFUNCTION(BlueprintCallable, Category = "CustomEvents")
	void GainKills(const FString& Victim);
	UPROPERTY(BlueprintAssignable, Category = "CustomEventsDispatcher")
	FOnKillEarnedDispatcher OnKillEarned;
public:
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "PlayerState")
	int PersonalScore;
	//Kills
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "PlayerState")
	int Kills;
	//Deaths
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "PlayerState")
	int Deaths;
	//TeamID
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "PlayerState")
	int TeamID;
};
