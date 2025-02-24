// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuUIManger.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
//widget switcher header
#include "Components/WidgetSwitcher.h"
// Sets default values
AMainMenuUIManger::AMainMenuUIManger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuUIManger::OnMainMenuTarBarClicked(int index)
{
	if (WidgetSwitcher)
	{
		//设置激活控件索引
		WidgetSwitcher->SetActiveWidgetIndex(index);
	}
}

void AMainMenuUIManger::InitMainMenuHUD()
{

}

// Called when the game starts or when spawned
void AMainMenuUIManger::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MainMenuUIManger BeginPlay"));
	if (!CurrentMenuHUD)
	{
		//使用计时器每隔0.5s尝试获取主菜单HUD
		GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([this]() {
			//获取主菜单HUD
			CurrentMenuHUD = GetWorld()->GetFirstPlayerController()->GetHUD();
			if (CurrentMenuHUD)
			{
				//清除定时器
				GetWorld()->GetTimerManager().ClearTimer(timerHandle);
				//创建主菜单控件
				CurrentMenuWidget = CreateWidget<UUserWidget>(GetWorld(), CurrentMenuWidgetClass);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Open MainMenuWidget"));
				CurrentMenuWidget->AddToViewport();

			}
			}), 0.1f, true);
	}
	else
	{
		//获取主菜单HUD
		CurrentMenuHUD = GetWorld()->GetFirstPlayerController()->GetHUD();
		//创建主菜单控件
		CurrentMenuWidget = CreateWidget<UUserWidget>(GetWorld(), CurrentMenuWidgetClass);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Open MainMenuWidget"));
		CurrentMenuWidget->AddToViewport();
	}
	//获取玩家控制器
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//设置输入模式仅限于UI
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
}

// Called every frame
void AMainMenuUIManger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

