// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameHUD.h"
#include "UI/HealthBarWidget.h"
#include "Blueprint/UserWidget.h"
AInGameHUD::AInGameHUD():Super()
{
}

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();
    // 确保控件已经在蓝图中创建，并且加载控件
    if (InGameHUDWidgetClass)
    {
        // 创建用户控件实例
        InGameHUDWidget = CreateWidget<UUserWidget>(GetWorld(), InGameHUDWidgetClass);

        // 添加到视口
        if (InGameHUDWidget)
        {
            InGameHUDWidget->AddToViewport();
        }
    }
}
