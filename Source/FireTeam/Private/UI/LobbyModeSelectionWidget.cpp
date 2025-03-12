// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyModeSelectionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/CustomData.h"
void ULobbyModeSelectionWidget::InitLobbyModeSelectionWidget()
{
	//��ȡ��Ϸģʽ���ݱ�
	GameModeTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(),nullptr,TEXT("/Game/_Game/Data/DT_GameModes")));
	// ��ȡ���ݱ�������
	RowNames = GameModeTable->GetRowNames();
	SelectedModeIndex = 0;
	//�ҵ���ΪTxt_GameMode��UTextBlock
	TextBlock_Mode = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_GameMode")));
	ModeImage = Cast<UImage>(GetWidgetFromName(TEXT("Mode_Image")));
}

void ULobbyModeSelectionWidget::OnClickedBtn_Next()
{
	SelectedModeIndex++;
	//�������������Χ������Ϊ0
	if (SelectedModeIndex >= RowNames.Num())
	{
		SelectedModeIndex = 0;
	}
}

void ULobbyModeSelectionWidget::OnClickedBtn_Prev()
{
	SelectedModeIndex--;
	//�������С��0������Ϊ���ֵ
	if (SelectedModeIndex < 0)
	{
		SelectedModeIndex = RowNames.Num() - 1;
	}
}

void ULobbyModeSelectionWidget::UpdateTextBlockMode()
{
	auto SelectedMode = RowNames[SelectedModeIndex];
	//����������ȡ��Ϸģʽ����
	FLobbyGameModeTableRow* Row = GameModeTable->FindRow<FLobbyGameModeTableRow>(SelectedMode, TEXT(""));
	//�����ı�
	TextBlock_Mode->SetText(Row->Name);
	ModeImage->SetBrushFromTexture(Row->Image);
	CurrentModePath = Row->ModePath;
}
