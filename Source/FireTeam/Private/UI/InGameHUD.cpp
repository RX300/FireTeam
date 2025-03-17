// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameHUD.h"
#include "UI/HealthBarWidget.h"
#include "UI/PlayerScoreBoardWidget.h"
#include "Blueprint/UserWidget.h"
#include <GameState/OnlineGameState.h>
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
        //在InGameHUDWidget里面找到PlayerScoreBoard
		PlayerScoreBoardWidget = Cast<UPlayerScoreBoardWidget>(InGameHUDWidget->GetWidgetFromName("PlayerScoreBoard"));
        if (PlayerScoreBoardWidget)
        {
			//让游戏状态绑定到PlayerScoreBoardWidget的OnPlayerJoined事件
			auto CurGameState = GetWorld()->GetGameState<AOnlineGameState>();
            if (CurGameState)
            {
                // 绑定玩家加入事件
                CurGameState->OnPlayerJoined.AddDynamic(PlayerScoreBoardWidget, &UPlayerScoreBoardWidget::OnPlayerJoined);
                // 绑定分数更新事件
                CurGameState->OnScoreUpdated.AddDynamic(PlayerScoreBoardWidget, &UPlayerScoreBoardWidget::OnScoreUpdated);
            }
            else
            {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InGameHUD BeginPlay：CurGameState is not Getted"));
            }
        }
        // 添加到视口
        if (InGameHUDWidget)
        {
            InGameHUDWidget->AddToViewport();
        }
    }
}
