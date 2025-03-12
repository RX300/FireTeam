// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyHUD.h"
#include "Blueprint/UserWidget.h"

void ALobbyHUD::BeginPlay()
{
    Super::BeginPlay();
    //// 确保控件已经在蓝图中创建，并且加载控件
    //if (LobbyMenuWidgetClass)
    //{
    //    // 创建用户控件实例
    //    LobbyMenuWidget = CreateWidget<UUserWidget>(GetWorld(), LobbyMenuWidgetClass);

    //    // 添加到视口
    //    if (LobbyMenuWidget)
    //    {
    //        LobbyMenuWidget->AddToViewport();
    //    }
    //}
}

void ALobbyHUD::DisplayLoadScreen()
{
	// 确保控件已经在蓝图中创建，并且加载控件
	if (LoadScreenWidgetClass)
	{
		// 创建用户控件实例
		LoadScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadScreenWidgetClass);
		// 添加到视口
		if (LoadScreenWidget)
		{
			LoadScreenWidget->AddToViewport();
		}
	}
    else
    {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LoadScreenWidgetClass is null!"));
    }
}
