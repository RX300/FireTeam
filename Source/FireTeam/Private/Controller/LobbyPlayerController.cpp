// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/LobbyPlayerController.h"
#include "UI/LobbyHUD.h"
void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//�ӿ������л�ȡ��LobbyHUD
	AsLobbyHUD = Cast<ALobbyHUD>(GetHUD());
}
