// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillAnnouncementWidget.h"
#include "Components/TextBlock.h"
void UKillAnnouncementWidget::Init()
{
	auto killUI = Cast<UTextBlock>(GetWidgetFromName(TEXT("KillerNameText")));
	if (killUI)
	{
		killUI->SetText(FText::FromString(KillerName.ToString()));
	}
	auto victimUI = Cast<UTextBlock>(GetWidgetFromName(TEXT("VictimNameText")));
	if (victimUI)
	{
		victimUI->SetText(FText::FromString(VictimName.ToString()));
	}
	//after 5s, clear self
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UKillAnnouncementWidget::ClearSelf, 5.f, false);
}

void UKillAnnouncementWidget::ClearSelf()
{
	//从父项中移除自己
	RemoveFromParent();
}
