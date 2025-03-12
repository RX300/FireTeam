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
	TSubclassOf<UUserWidget> InGameHUDWidgetClass;  // 引用蓝图类
	TObjectPtr<UUserWidget> InGameHUDWidget;  // 这是指向用户控件的指针
};
