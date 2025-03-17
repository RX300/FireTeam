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
	// ��Ҽ�����Ϸʱ�Ļص�����
	UFUNCTION()
	void OnPlayerJoined(APlayerState* NewPlayerState);
	// ������ҷ���
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerScore(const FString& playerName, int32 playerScore);

	// �ԼǷְ�������򲢸�����ʾ
	UFUNCTION(BlueprintCallable)
	void SortAndUpdateScoreBoard();
	// ��Ӧ���������¼��Ļص�����
	UFUNCTION(BlueprintCallable)
	void OnScoreUpdated(const FScoreData& ScoreData);
public:
	//vertical box-AllPlayersList
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> AllPlayersList;
	// ������ҼǷֿ���Widget��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPlayerScoreBoardCardWidget> PlayerScoreCardClass;
private:
	TArray<UPlayerScoreBoardCardWidget*> PlayerScoreCards;
};
