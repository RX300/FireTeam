// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
class UUserWidget;
UCLASS()
class FIRETEAM_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbyMenuWidgetClass;  // ������ͼ��
private:
	TObjectPtr<UUserWidget> LobbyMenuWidget;  // ����ָ���û��ؼ���ָ��

};
