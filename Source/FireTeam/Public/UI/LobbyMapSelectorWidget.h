// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMapSelectorWidget.generated.h"

/**
 * 
 */
class UDataTable;
class UTextBlock;
class UImage;

UCLASS()
class FIRETEAM_API ULobbyMapSelectorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//蓝图可调用的初始化函数
	UFUNCTION(BlueprintCallable)
	void InitMapSelectionWidget();
	UFUNCTION(BlueprintCallable)
	void OnClickedBtn_Next();
	UFUNCTION(BlueprintCallable)
	void OnClickedBtn_Prev();
	UFUNCTION(BlueprintCallable)
	void UpdateMap();
private:
	TObjectPtr<UDataTable> GameModeTable;
public:
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TObjectPtr<UTextBlock> TextBlock_Mode;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TArray<FName> RowNames;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int SelectedMapIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TObjectPtr<UImage> MapImage;
};
