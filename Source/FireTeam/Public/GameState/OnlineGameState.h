// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OnlineGameState.generated.h"

/**
 * 
 */
class UKillAnnouncementWidget;

UCLASS()
class FIRETEAM_API AOnlineGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UFUNCTION(Server,Reliable, BlueprintCallable, Category = "Custom Events")
	void Server_AnnounceKill(APlayerState* playerstate,const FString& Victim);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Custom Events")
	void NetMulticast_KillAnnounced(APlayerState* playerstate, const FString& Victim);
	//NewPlayerJoined,param:APlayerController
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void NewPlayerJoined(APlayerController* NewPlayer);
public:
	//KillAnnouncementWidgetClass
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UKillAnnouncementWidget> KillAnnouncementWidgetClass;
};
