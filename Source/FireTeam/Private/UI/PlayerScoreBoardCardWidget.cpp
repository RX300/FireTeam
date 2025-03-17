// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerScoreBoardCardWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "GameState/OnlineGameState.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerScoreBoardCardWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	PositionText->SetText(PositionTextContent);
}
void UPlayerScoreBoardCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurGameState = GetWorld()->GetGameState<AOnlineGameState>();
	//如果curGameState为空，则每隔0.5秒获取一次，直到获取到
	if (!CurGameState)
	{
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this]()
			{
				CurGameState = GetWorld()->GetGameState<AOnlineGameState>();
				if (CurGameState)
				{
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				}
			});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, true);
	}
}

void UPlayerScoreBoardCardWidget::UpdateUI(const FString& playerName, int32 playerScore)
{
	PlayerName = playerName;
	PlayerScore = playerScore;
	PlayerNameText->SetText(FText::FromString(playerName));
	PlayerScoreText->SetText(FText::AsNumber(playerScore));
    //print CurGameState->WinThreshold
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("WinThreshold is %d"), CurGameState->WinThreshold));
	ProgressBar->SetPercent(UKismetMathLibrary::NormalizeToRange(playerScore, 0, CurGameState->WinThreshold));
}

void UPlayerScoreBoardCardWidget::SetPosition(int32 Position)
{
    // 生成位置后缀
    FString Suffix;
    if (Position % 10 == 1 && Position % 100 != 11)
        Suffix = TEXT("st");
    else if (Position % 10 == 2 && Position % 100 != 12)
        Suffix = TEXT("nd");
    else if (Position % 10 == 3 && Position % 100 != 13)
        Suffix = TEXT("rd");
    else
        Suffix = TEXT("th");

    // 设置位置文本
    FString PositionString = FString::Printf(TEXT("%d%s"), Position, *Suffix);
    PositionText->SetText(FText::FromString(PositionString));
}
