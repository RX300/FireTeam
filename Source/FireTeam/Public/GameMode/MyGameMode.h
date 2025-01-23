// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIRETEAM_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMyGameMode();
	//生命AActor的常用生命周期函数
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaSeconds) override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

//~ Begin AGameModeBase Interface
	//virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	//virtual void StartPlay() override;
	//virtual bool HasMatchStarted() const override;
/** Returns true if the match state is WaitingPostMatch or later */
	//virtual bool HasMatchEnded() const override;
	//virtual void PostLogin(APlayerController* NewPlayer) override;
	//virtual void Logout(AController* Exiting) override;
	//virtual int32 GetNumPlayers() override;
	//virtual int32 GetNumSpectators() override;
	//virtual bool IsHandlingReplays() override;
	//virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	//virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	//virtual void PostSeamlessTravel() override;
	//virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	//virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	//virtual bool CanServerTravel(const FString& URL, bool bAbsolute) override;
	//virtual void StartToLeaveMap() override;
	//virtual bool SpawnPlayerFromSimulate(const FVector& NewLocation, const FRotator& NewRotation) override;
//~ End AGameModeBase Interface
};
