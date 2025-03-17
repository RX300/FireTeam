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
    // ȷ���ؼ��Ѿ�����ͼ�д��������Ҽ��ؿؼ�
    if (InGameHUDWidgetClass)
    {
        // �����û��ؼ�ʵ��
        InGameHUDWidget = CreateWidget<UUserWidget>(GetWorld(), InGameHUDWidgetClass);
        //��InGameHUDWidget�����ҵ�PlayerScoreBoard
		PlayerScoreBoardWidget = Cast<UPlayerScoreBoardWidget>(InGameHUDWidget->GetWidgetFromName("PlayerScoreBoard"));
        if (PlayerScoreBoardWidget)
        {
			//����Ϸ״̬�󶨵�PlayerScoreBoardWidget��OnPlayerJoined�¼�
			auto CurGameState = GetWorld()->GetGameState<AOnlineGameState>();
            if (CurGameState)
            {
                // ����Ҽ����¼�
                CurGameState->OnPlayerJoined.AddDynamic(PlayerScoreBoardWidget, &UPlayerScoreBoardWidget::OnPlayerJoined);
                // �󶨷��������¼�
                CurGameState->OnScoreUpdated.AddDynamic(PlayerScoreBoardWidget, &UPlayerScoreBoardWidget::OnScoreUpdated);
            }
            else
            {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InGameHUD BeginPlay��CurGameState is not Getted"));
            }
        }
        // ��ӵ��ӿ�
        if (InGameHUDWidget)
        {
            InGameHUDWidget->AddToViewport();
        }
    }
}
