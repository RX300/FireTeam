// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerScoreBoardWidget.h"
#include "UI/PlayerScoreBoardCardWidget.h"
#include "PlayerState/OnlinePlayerState.h"
#include "Components/VerticalBox.h"
#include "PlayerState/OnlinePlayerState.h"
#include "GameState/OnlineGameState.h"

void UPlayerScoreBoardWidget::UpdateUI(const FString& playerName, int32 playerScore)
{

}

void UPlayerScoreBoardWidget::OnPlayerJoined(APlayerState* NewPlayerState)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnPlayerJoined"));
    if (NewPlayerState)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("NewPlayerState is Getted"));
        AOnlinePlayerState* CurPlayerState = Cast<AOnlinePlayerState>(NewPlayerState);
        if (CurPlayerState)
        {
            if (!PlayerScoreCardClass)
            {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("PlayerScoreCardClass is not Getted"));
            }

            // 创建新的玩家记分卡
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Create Score Widget"));
            UPlayerScoreBoardCardWidget* NewCard = CreateWidget<UPlayerScoreBoardCardWidget>(GetWorld(), PlayerScoreCardClass);
            if (NewCard)
            {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("NewCard is Getted"));
                // 添加到垂直框中
                AllPlayersList->AddChild(NewCard);
				PlayerScoreCards.Add(NewCard);
                // 初始化记分卡UI
				auto playerName = NewPlayerState->GetPlayerName();
                NewCard->UpdateUI(playerName, 0);
				SortAndUpdateScoreBoard();
            }

        }
    }
}

void UPlayerScoreBoardWidget::UpdatePlayerScore(const FString& playerName, int32 playerScore)
{
    // 查找玩家卡片并更新分数
    for (UPlayerScoreBoardCardWidget* Card : PlayerScoreCards)
    {
        if (Card && Card->GetPlayerName() == playerName)
        {
            Card->UpdateUI(playerName, playerScore);
            SortAndUpdateScoreBoard();
            return;
        }
    }
}

void UPlayerScoreBoardWidget::SortAndUpdateScoreBoard()
{
    // 排序玩家卡片
    PlayerScoreCards.Sort([](const UPlayerScoreBoardCardWidget& A, const UPlayerScoreBoardCardWidget& B) {
        // 首先按分数降序排序
        if (A.GetPlayerScore() != B.GetPlayerScore())
            return A.GetPlayerScore() > B.GetPlayerScore();

        // 如果分数相同，按名称升序排序
        return A.GetPlayerName() < B.GetPlayerName();
        });

    // 清空垂直框
    AllPlayersList->ClearChildren();

    // 重新添加排序后的卡片并更新排名
    for (int32 i = 0; i < PlayerScoreCards.Num(); ++i)
    {
        UPlayerScoreBoardCardWidget* Card = PlayerScoreCards[i];
        if (Card)
        {
            AllPlayersList->AddChild(Card);
            Card->SetPosition(i + 1);  // 设置排名，从1开始
        }
    }
}

void UPlayerScoreBoardWidget::OnScoreUpdated(const FScoreData& ScoreData)
{
    // 更新所有玩家的分数
    for (const auto& ScorePair : ScoreData.PlayerScores)
    {
        int32 PlayerId = ScorePair.Key;
        int32 Score = ScorePair.Value;

        // 查找对应的PlayerState
        AOnlineGameState* GameState = GetWorld()->GetGameState<AOnlineGameState>();
        if (GameState)
        {
            // 遍历所有PlayerState，找到匹配的ID
            for (APlayerState* PlayerState : GameState->ConnectedPlayerArray)
            {
                if (PlayerState && PlayerState->GetPlayerId() == PlayerId)
                {
                    // 找到匹配的PlayerState，更新分数
                    UpdatePlayerScore(PlayerState->GetPlayerName(), Score);
                    break;
                }
            }
        }
    }
}
