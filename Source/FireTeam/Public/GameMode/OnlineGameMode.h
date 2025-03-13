// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OnlineGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIRETEAM_API AOnlineGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	//事件后台登陆的接口
	virtual void PostLogin(APlayerController* NewPlayer) override;
	//交换玩家控制器时的接口 SwapPlayerControllers
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)override;

public:
	//蓝图可调用的RespawnPlayer函数，并且设置为虚函数
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void RespawnPlayer(AController* Controller);
};
