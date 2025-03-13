// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillAnnouncementTrayWidget.generated.h"

/**
 * 
 */
class UVerticalBox;
class UKillAnnouncementWidget;
UCLASS()
class FIRETEAM_API UKillAnnouncementTrayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void AddAnnouncement(UKillAnnouncementWidget* killAnouncement);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KillAnouncementTrayWidget", meta = (BindWidget))
	TObjectPtr<UVerticalBox> AnnouncementList;
};
