// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyMapSelectorWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/CustomData.h"

void ULobbyMapSelectorWidget::InitMapSelectionWidget()
{
	//获取游戏模式数据表
	GameModeTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/_Game/Data/DT_GameMaps")));
	// 获取数据表行名称
	RowNames = GameModeTable->GetRowNames();
	SelectedMapIndex = 0;
	//找到名为Txt_GameMode的UTextBlock
	TextBlock_Mode = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_GameMode")));
	MapImage = Cast<UImage>(GetWidgetFromName(TEXT("Map_Image")));
}

void ULobbyMapSelectorWidget::OnClickedBtn_Next()
{
	SelectedMapIndex++;
	//如果索引超出范围，重置为0
	if (SelectedMapIndex >= RowNames.Num())
	{
		SelectedMapIndex = 0;
	}
}

void ULobbyMapSelectorWidget::OnClickedBtn_Prev()
{
	SelectedMapIndex--;
	//如果索引小于0，重置为最大值
	if (SelectedMapIndex < 0)
	{
		SelectedMapIndex = RowNames.Num() - 1;
	}
}

void ULobbyMapSelectorWidget::UpdateMap()
{
	auto SelectedMap = RowNames[SelectedMapIndex];
	//根据索引获取游戏地图名称,2D纹理
	FGameMapTableRow* Row = GameModeTable->FindRow<FGameMapTableRow>(SelectedMap, TEXT(""));
	//更新文本和图片
	TextBlock_Mode->SetText(Row->Name);
	MapImage->SetBrushFromTexture(Row->Image);
	CurrentMapPath = Row->MapPath;
}
