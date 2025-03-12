// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/LobbyGameState.h"
#include "UI/LobbyHUD.h"

void ALobbyGameState::Multicast_LobbyDisplayLoadScreen_Implementation()
{
	//获取index = 0的玩家控制器
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//获取玩家控制器的HUD
	ALobbyHUD* HUD = Cast<ALobbyHUD>(PlayerController->GetHUD());
	//调用HUD的DisplayLoadScreen函数
	HUD->DisplayLoadScreen();
}

void ALobbyGameState::Server_LobbyDisplayLoadScreen_Implementation()
{
	//调用组播函数
	Multicast_LobbyDisplayLoadScreen();
}
