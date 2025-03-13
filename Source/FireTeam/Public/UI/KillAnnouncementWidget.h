// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillAnnouncementWidget.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class FIRETEAM_API UKillAnnouncementWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void Init();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void ClearSelf();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kill Info")
	FText KillerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kill Info")
	FText VictimName;
	//KillerNameUI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Setting")
	TObjectPtr<UTextBlock> KillerNameUI;
	//VictimNameUI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Setting")
	TObjectPtr<UTextBlock> VictimNameUI;
private:
	//¶¨Ê±Æ÷
	FTimerHandle TimerHandle;
};
