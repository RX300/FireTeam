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
    // ȷ���ؼ��Ѿ�����ͼ�д��������Ҽ��ؿؼ�
    if (InGameHUDWidgetClass)
    {
        // �����û��ؼ�ʵ��
        InGameHUDWidget = CreateWidget<UUserWidget>(GetWorld(), InGameHUDWidgetClass);

        // ��ӵ��ӿ�
        if (InGameHUDWidget)
        {
            InGameHUDWidget->AddToViewport();
        }
    }
}
