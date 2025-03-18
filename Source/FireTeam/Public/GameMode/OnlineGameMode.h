// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OnlineGameMode.generated.h"
//��̬�ಥί�У�2��������param1:int winnerPlayerID��param2:int winnerTeamID
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMatchEnd, int, winnerPlayerID, int, winnerTeamID);
/**
 * 
 */
UCLASS()
class FIRETEAM_API AOnlineGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	//�¼���̨��½�Ľӿ�
	virtual void PostLogin(APlayerController* NewPlayer) override;
	//������ҿ�����ʱ�Ľӿ� SwapPlayerControllers
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)override;
	virtual void EndMatch()override;
protected:
	//override ReadyToEndMatch
	virtual bool ReadyToEndMatch_Implementation() override;
public:
	//��ͼ�ɵ��õ�RespawnPlayer��������������Ϊ�麯��
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
	//�Ƿ����Ŷ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
	bool isTeamType;
};
