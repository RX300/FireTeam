// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyModeSelectionWidget.generated.h"

/**
 * 
 */
class UDataTable;
class UTextBlock;
class UImage;
UCLASS()
class FIRETEAM_API ULobbyModeSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//��ͼ�ɵ��õĳ�ʼ������
	UFUNCTION(BlueprintCallable)
	void InitLobbyModeSelectionWidget();
	UFUNCTION(BlueprintCallable)
	void OnClickedBtn_Next();
	UFUNCTION(BlueprintCallable)
	void OnClickedBtn_Prev();
	UFUNCTION(BlueprintCallable)
	void UpdateTextBlockMode();
private:
	TObjectPtr<UDataTable> GameModeTable;
public:
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TObjectPtr<UTextBlock> TextBlock_Mode;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TArray<FName> RowNames;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int SelectedModeIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TObjectPtr<UImage> ModeImage;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	FString CurrentModePath;
};
