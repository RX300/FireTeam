// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OnlineGameState.generated.h"

USTRUCT(BlueprintType)
struct FScoreData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TMap<int32, int32> PlayerScores;

	UPROPERTY(BlueprintReadWrite)
	TMap<int32, int32> TeamScores;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam
(FOnScoreUpdated, 
const FScoreData&, ScoreData);

/**
 * 
 */
class UKillAnnouncementWidget;

UCLASS()
class FIRETEAM_API AOnlineGameState : public AGameState
{
	GENERATED_BODY()
public:
	UFUNCTION(Server,Reliable, BlueprintCallable, Category = "Custom Events")
	void Server_AnnounceKill(APlayerState* playerstate,const FString& Victim);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Custom Events")
	virtual void NetMulticast_KillAnnounced(APlayerState* playerstate, const FString& Victim);
	//NewPlayerJoined,param:APlayerController
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void NewPlayerJoined(APlayerController* NewPlayer);
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void AddPlayerPoint(int PlayerID, int Score);
	FOnScoreUpdated OnScoreUpdated;
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void AddTeamPoint(int TeamID, int Score);
public:
	//KillAnnouncementWidgetClass
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UKillAnnouncementWidget> KillAnnouncementWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TMap<int32, int32> PlayerScoreBoard;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TMap<int32, int32> TeamScoreBoard;
};
