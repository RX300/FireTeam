// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OnlineGameMode.generated.h"

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
protected:
	//override ReadyToEndMatch
	virtual bool ReadyToEndMatch() ;
public:
	//��ͼ�ɵ��õ�RespawnPlayer��������������Ϊ�麯��
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void RespawnPlayer(AController* Controller);
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
