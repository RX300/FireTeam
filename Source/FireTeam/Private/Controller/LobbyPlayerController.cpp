// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/LobbyPlayerController.h"
#include "UI/LobbyHUD.h"
void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//从控制器中获取到LobbyHUD
	AsLobbyHUD = Cast<ALobbyHUD>(GetHUD());
}
