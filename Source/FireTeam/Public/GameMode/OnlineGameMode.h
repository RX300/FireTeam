// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OnlineGameMode.generated.h"
//动态多播委托，2个参数，param1:int winnerPlayerID，param2:int winnerTeamID
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMatchEnd, int, winnerPlayerID, int, winnerTeamID);
/**
 * 
 */
UCLASS()
class FIRETEAM_API AOnlineGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	//事件后台登陆的接口
	virtual void PostLogin(APlayerController* NewPlayer) override;
	//交换玩家控制器时的接口 SwapPlayerControllers
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)override;
	virtual void EndMatch()override;
protected:
	//override ReadyToEndMatch
	virtual bool ReadyToEndMatch_Implementation() override;
public:
	//蓝图可调用的RespawnPlayer函数，并且设置为虚函数
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void RespawnPlayer(AController* Controller);
	FOnMatchEnd OnMatchEnd;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
	int32 WinThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
	int32 WinnerID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
	int32 WinningTeamID;
	//是否是团队类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
	bool isTeamType;
};
