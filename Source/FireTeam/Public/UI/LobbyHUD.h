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
	TSubclassOf<UUserWidget> LobbyMenuWidgetClass;  // 引用蓝图类
private:
	TObjectPtr<UUserWidget> LobbyMenuWidget;  // 这是指向用户控件的指针

};
