// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
class UUserWidget;
UCLASS()
class FIRETEAM_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	AInGameHUD();
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InGameHUDWidgetClass;  // ������ͼ��
	TObjectPtr<UUserWidget> InGameHUDWidget;  // ����ָ���û��ؼ���ָ��
};
