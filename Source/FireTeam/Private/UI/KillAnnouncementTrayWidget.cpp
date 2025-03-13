// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillAnnouncementTrayWidget.h"
#include "UI/KillAnnouncementWidget.h"
#include "Components/VerticalBox.h"
void UKillAnnouncementTrayWidget::AddAnnouncement(UKillAnnouncementWidget* killAnouncement)
{
	if (AnnouncementList)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Add KillAnnouncementTrayWidget Child"));
		AnnouncementList->AddChild(killAnouncement);
	}
}
