// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/LobbyGameState.h"
#include "UI/LobbyHUD.h"

void ALobbyGameState::Multicast_LobbyDisplayLoadScreen_Implementation()
{
	//��ȡindex = 0����ҿ�����
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//��ȡ��ҿ�������HUD
	ALobbyHUD* HUD = Cast<ALobbyHUD>(PlayerController->GetHUD());
	//����HUD��DisplayLoadScreen����
	HUD->DisplayLoadScreen();
}

void ALobbyGameState::Server_LobbyDisplayLoadScreen_Implementation()
{
	//�����鲥����
	Multicast_LobbyDisplayLoadScreen();
}
