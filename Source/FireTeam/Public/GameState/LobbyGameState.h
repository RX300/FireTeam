// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class FIRETEAM_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	//�鲥��DisplayLoadScreen�¼�
	UFUNCTION(NetMulticast, Reliable,BlueprintCallable, Category = "Event")
	void Multicast_LobbyDisplayLoadScreen();
	//Server��DisplayLoadScreen�¼�
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Event")
	void Server_LobbyDisplayLoadScreen();
	
};
