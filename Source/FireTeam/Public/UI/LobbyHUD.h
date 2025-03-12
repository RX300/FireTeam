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
	UFUNCTION(BlueprintCallable, Category = "UI")
	void DisplayLoadScreen();
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbyMenuWidgetClass;  // 引用蓝图类
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;  // 表示LoadScreen的类
private:
	TObjectPtr<UUserWidget> LobbyMenuWidget;  // 这是指向LobbyMenu的指针
	TObjectPtr<UUserWidget> LoadScreenWidget;  // 这是指向 LoadScreen的指针
};
