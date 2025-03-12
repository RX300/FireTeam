// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyModeSelectionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/CustomData.h"
void ULobbyModeSelectionWidget::InitLobbyModeSelectionWidget()
{
	//获取游戏模式数据表
	GameModeTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(),nullptr,TEXT("/Game/_Game/Data/DT_GameModes")));
	// 获取数据表行名称
	RowNames = GameModeTable->GetRowNames();
	SelectedModeIndex = 0;
	//找到名为Txt_GameMode的UTextBlock
	TextBlock_Mode = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_GameMode")));
	ModeImage = Cast<UImage>(GetWidgetFromName(TEXT("Mode_Image")));
}

void ULobbyModeSelectionWidget::OnClickedBtn_Next()
{
	SelectedModeIndex++;
	//如果索引超出范围，重置为0
	if (SelectedModeIndex >= RowNames.Num())
	{
		SelectedModeIndex = 0;
	}
}

void ULobbyModeSelectionWidget::OnClickedBtn_Prev()
{
	SelectedModeIndex--;
	//如果索引小于0，重置为最大值
	if (SelectedModeIndex < 0)
	{
		SelectedModeIndex = RowNames.Num() - 1;
	}
}

void ULobbyModeSelectionWidget::UpdateTextBlockMode()
{
	auto SelectedMode = RowNames[SelectedModeIndex];
	//根据索引获取游戏模式名称
	FLobbyGameModeTableRow* Row = GameModeTable->FindRow<FLobbyGameModeTableRow>(SelectedMode, TEXT(""));
	//更新文本
	TextBlock_Mode->SetText(Row->Name);
	ModeImage->SetBrushFromTexture(Row->Image);
	CurrentModePath = Row->ModePath;
}
