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
	//�¼���̨��½�Ľӿ�
	virtual void PostLogin(APlayerController* NewPlayer) override;
	//������ҿ�����ʱ�Ľӿ� SwapPlayerControllers
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)override;

public:
	//��ͼ�ɵ��õ�RespawnPlayer��������������Ϊ�麯��
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void RespawnPlayer(AController* Controller);
};
