// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyMapSelectorWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/CustomData.h"

void ULobbyMapSelectorWidget::InitMapSelectionWidget()
{
	//��ȡ��Ϸģʽ���ݱ�
	GameModeTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/_Game/Data/DT_GameMaps")));
	// ��ȡ���ݱ�������
	RowNames = GameModeTable->GetRowNames();
	SelectedMapIndex = 0;
	//�ҵ���ΪTxt_GameMode��UTextBlock
	TextBlock_Mode = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_GameMode")));
	MapImage = Cast<UImage>(GetWidgetFromName(TEXT("Map_Image")));
}

void ULobbyMapSelectorWidget::OnClickedBtn_Next()
{
	SelectedMapIndex++;
	//�������������Χ������Ϊ0
	if (SelectedMapIndex >= RowNames.Num())
	{
		SelectedMapIndex = 0;
	}
}

void ULobbyMapSelectorWidget::OnClickedBtn_Prev()
{
	SelectedMapIndex--;
	//�������С��0������Ϊ���ֵ
	if (SelectedMapIndex < 0)
	{
		SelectedMapIndex = RowNames.Num() - 1;
	}
}

void ULobbyMapSelectorWidget::UpdateMap()
{
	auto SelectedMap = RowNames[SelectedMapIndex];
	//����������ȡ��Ϸ��ͼ����,2D����
	FGameMapTableRow* Row = GameModeTable->FindRow<FGameMapTableRow>(SelectedMap, TEXT(""));
	//�����ı���ͼƬ
	TextBlock_Mode->SetText(Row->Name);
	MapImage->SetBrushFromTexture(Row->Image);
	CurrentMapPath = Row->MapPath;
}
