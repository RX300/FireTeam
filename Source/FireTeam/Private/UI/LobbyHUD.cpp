// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyHUD.h"
#include "Blueprint/UserWidget.h"

void ALobbyHUD::BeginPlay()
{
    Super::BeginPlay();
    //// ȷ���ؼ��Ѿ�����ͼ�д��������Ҽ��ؿؼ�
    //if (LobbyMenuWidgetClass)
    //{
    //    // �����û��ؼ�ʵ��
    //    LobbyMenuWidget = CreateWidget<UUserWidget>(GetWorld(), LobbyMenuWidgetClass);

    //    // ��ӵ��ӿ�
    //    if (LobbyMenuWidget)
    //    {
    //        LobbyMenuWidget->AddToViewport();
    //    }
    //}
}

void ALobbyHUD::DisplayLoadScreen()
{
	// ȷ���ؼ��Ѿ�����ͼ�д��������Ҽ��ؿؼ�
	if (LoadScreenWidgetClass)
	{
		// �����û��ؼ�ʵ��
		LoadScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadScreenWidgetClass);
		// ��ӵ��ӿ�
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
