// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScoreBoardWidget.generated.h"

/**
 * 
 */
class UVerticalBox;
class UPlayerScoreBoardCardWidget;
struct FScoreData;
UCLASS()
class FIRETEAM_API UPlayerScoreBoardWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UpdateUI
	UFUNCTION(BlueprintCallable)
	void UpdateUI(const FString&playerName,int32 playerScore);
	// 玩家加入游戏时的回调函数
	UFUNCTION()
	void OnPlayerJoined(APlayerState* NewPlayerState);
	// 更新玩家分数
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerScore(const FString& playerName, int32 playerScore);

	// 对记分板进行排序并更新显示
	UFUNCTION(BlueprintCallable)
	void SortAndUpdateScoreBoard();
	// 响应分数更新事件的回调函数
	UFUNCTION(BlueprintCallable)
	void OnScoreUpdated(const FScoreData& ScoreData);
public:
	//vertical box-AllPlayersList
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> AllPlayersList;
	// 设置玩家记分卡的Widget类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPlayerScoreBoardCardWidget> PlayerScoreCardClass;
private:
	TArray<UPlayerScoreBoardCardWidget*> PlayerScoreCards;
};
