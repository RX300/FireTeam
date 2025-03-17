// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScoreBoardCardWidget.generated.h"

/**
 * 
 */
class URichTextBlock;
class UTextBlock;
class UProgressBar;
class AOnlineGameState;
UCLASS()
class FIRETEAM_API UPlayerScoreBoardCardWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// override NativePreConstruct
	void NativePreConstruct();
	// override NativeConstruct
	virtual void NativeConstruct() override;
public:
	//UpdateUI
	UFUNCTION(BlueprintCallable)
	void UpdateUI(const FString& playerName, int32 playerScore);
	// 获取玩家名称
	UFUNCTION(BlueprintCallable)
	FString GetPlayerName() const { return PlayerName; }

	// 获取玩家分数
	UFUNCTION(BlueprintCallable)
	int32 GetPlayerScore() const { return PlayerScore; }

	// 设置位置文本
	UFUNCTION(BlueprintCallable)
	void SetPosition(int32 Position);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> PositionText;
	//Rich Text 对应的文本内容,默认是self
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PositionTextContent = FText::FromString("Self");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerScoreText;
	//ProgressBar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
	//Save GameState
	UPROPERTY()
	TObjectPtr<AOnlineGameState> CurGameState;
private:
	// 存储玩家名称和分数
	FString PlayerName;
	int32 PlayerScore = 0;
	FTimerHandle TimerHandle;
};
