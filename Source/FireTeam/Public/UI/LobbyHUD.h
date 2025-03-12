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
	TSubclassOf<UUserWidget> LobbyMenuWidgetClass;  // ������ͼ��
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;  // ��ʾLoadScreen����
private:
	TObjectPtr<UUserWidget> LobbyMenuWidget;  // ����ָ��LobbyMenu��ָ��
	TObjectPtr<UUserWidget> LoadScreenWidget;  // ����ָ�� LoadScreen��ָ��
};
