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

            // �����µ���ҼǷֿ�
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Create Score Widget"));
            UPlayerScoreBoardCardWidget* NewCard = CreateWidget<UPlayerScoreBoardCardWidget>(GetWorld(), PlayerScoreCardClass);
            if (NewCard)
            {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("NewCard is Getted"));
                // ��ӵ���ֱ����
                AllPlayersList->AddChild(NewCard);
				PlayerScoreCards.Add(NewCard);
                // ��ʼ���Ƿֿ�UI
				auto playerName = NewPlayerState->GetPlayerName();
                NewCard->UpdateUI(playerName, 0);
				SortAndUpdateScoreBoard();
            }

        }
    }
}

void UPlayerScoreBoardWidget::UpdatePlayerScore(const FString& playerName, int32 playerScore)
{
    // ������ҿ�Ƭ�����·���
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
    // ������ҿ�Ƭ
    PlayerScoreCards.Sort([](const UPlayerScoreBoardCardWidget& A, const UPlayerScoreBoardCardWidget& B) {
        // ���Ȱ�������������
        if (A.GetPlayerScore() != B.GetPlayerScore())
            return A.GetPlayerScore() > B.GetPlayerScore();

        // ���������ͬ����������������
        return A.GetPlayerName() < B.GetPlayerName();
        });

    // ��մ�ֱ��
    AllPlayersList->ClearChildren();

    // ������������Ŀ�Ƭ����������
    for (int32 i = 0; i < PlayerScoreCards.Num(); ++i)
    {
        UPlayerScoreBoardCardWidget* Card = PlayerScoreCards[i];
        if (Card)
        {
            AllPlayersList->AddChild(Card);
            Card->SetPosition(i + 1);  // ������������1��ʼ
        }
    }
}

void UPlayerScoreBoardWidget::OnScoreUpdated(const FScoreData& ScoreData)
{
    // ����������ҵķ���
    for (const auto& ScorePair : ScoreData.PlayerScores)
    {
        int32 PlayerId = ScorePair.Key;
        int32 Score = ScorePair.Value;

        // ���Ҷ�Ӧ��PlayerState
        AOnlineGameState* GameState = GetWorld()->GetGameState<AOnlineGameState>();
        if (GameState)
        {
            // ��������PlayerState���ҵ�ƥ���ID
            for (APlayerState* PlayerState : GameState->ConnectedPlayerArray)
            {
                if (PlayerState && PlayerState->GetPlayerId() == PlayerId)
                {
                    // �ҵ�ƥ���PlayerState�����·���
                    UpdatePlayerScore(PlayerState->GetPlayerName(), Score);
                    break;
                }
            }
        }
    }
}
