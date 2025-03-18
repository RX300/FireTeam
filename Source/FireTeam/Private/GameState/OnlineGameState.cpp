// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/OnlineGameState.h"
#include "GameMode/OnlineGameMode.h"
#include "UI/KillAnnouncementWidget.h"
#include "UI/KillAnnouncementTrayWidget.h"
#include "UI/InGameHUD.h"
#include "GameFramework/PlayerState.h" // 添加此行以解决不完整类型错误
#include "PlayerState/OnlinePlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h" // 使用DOREPLIFETIME必须包含此头文件
void AOnlineGameState::Server_AnnounceKill_Implementation(APlayerState* playerstate, const FString& Victim)
{
	NetMulticast_KillAnnounced(playerstate, Victim);
}

void AOnlineGameState::NetMulticast_KillAnnounced_Implementation(APlayerState* playerstate, const FString& Victim)
{
	//创建KillAnnouncementWidget的UI控件
	TObjectPtr<UKillAnnouncementWidget> KillAnnouncementWidget = CreateWidget<UKillAnnouncementWidget>(GetWorld(), KillAnnouncementWidgetClass);
	KillAnnouncementWidget->KillerName = FText::FromString(playerstate->GetPlayerName());
	KillAnnouncementWidget->VictimName = FText::FromString(Victim);
	KillAnnouncementWidget->Init();
	//通过玩家控制器获取到HUD，再转换到InGameHUD获取到KillAnnouncementTrayWidget，把KillAnnouncementWidget添加到KillAnnouncementTrayWidget中
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		AInGameHUD* InGameHUD = Cast<AInGameHUD>(PlayerController->GetHUD());
		if (InGameHUD)
		{
			UKillAnnouncementTrayWidget* KillAnnouncementTrayWidget = Cast<UKillAnnouncementTrayWidget>(InGameHUD->InGameHUDWidget->GetWidgetFromName("KillAnnouncementTray"));
			if (KillAnnouncementTrayWidget)
			{
				KillAnnouncementTrayWidget->AddAnnouncement(KillAnnouncementWidget);
			}
		}
	}
}

void AOnlineGameState::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority())
	{
		return;
	}
	//Get GameMode
	AOnlineGameMode* CurGameMode = GetWorld()->GetAuthGameMode<AOnlineGameMode>();
	CurGameMode->OnMatchEnd.AddDynamic(this, &AOnlineGameState::OnMatchEnded);
}

void AOnlineGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 设置一个定时器，每隔一小段时间检查委托是否已绑定，然后广播缓存的玩家加入事件
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOnlineGameState::BroadcastPendingPlayerJoins, 0.5f, true);
}

void AOnlineGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOnlineGameState, ConnectedPlayerArray);
	DOREPLIFETIME(AOnlineGameState, WinnerID);
	DOREPLIFETIME(AOnlineGameState, WinningTeamID);
}

//在GameMode里调用，GameMode仅仅只在服务器端存在
void AOnlineGameState::NewPlayerJoined(APlayerController* NewPlayer)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Player Joined"));

	if (!NewPlayer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NewPlayer is null"));
		return;
	}

	// 创建一个检查PlayerState的TimerDelegate
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this, NewPlayer]()
		{
			if (!NewPlayer)
				return;

			APlayerState* PlayerState = NewPlayer->PlayerState;
			if (!PlayerState)
			{
				return; // 继续等待PlayerState初始化
			}
			ConnectedPlayerArray.Add(PlayerState);

			// PlayerState已初始化，可以处理
			AOnlinePlayerState* OnlinePlayerState = Cast<AOnlinePlayerState>(PlayerState);
			if (OnlinePlayerState)
			{
				OnlinePlayerState->OnKillEarned.AddDynamic(this, &AOnlineGameState::Server_AnnounceKill);
			}

			// 打印当前PlayerArray信息
			GEngine->AddOnScreenDebugMessage(-1, 105.f, FColor::Yellow,
				FString::Printf(TEXT("PlayerState ready: Name=%s, ConnectedPlayerArray.Num=%d"),
					*PlayerState->GetPlayerName(), ConnectedPlayerArray.Num()));

			// 使用网络多播函数通知所有客户端
			Multicast_NewPlayerJoined(PlayerState);

			// 清除定时器
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		});

	// 每0.5秒检查一次，直到PlayerState初始化完成
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, true);
}

void AOnlineGameState::Multicast_NewPlayerJoined_Implementation(APlayerState* NewPlayerState)
{
	// 如果委托已绑定，直接广播；否则缓存起来
	if (HasPlayerJoinedDelegates())
	{
		OnPlayerJoined.Broadcast(NewPlayerState);
	}
	else
	{
		// 缓存待广播的玩家
		PendingJoinedPlayers.AddUnique(NewPlayerState);
	}
	//遍历GameState的PlayerArray，调用OnPlayerJoined	
	if (!isInitializedNewPlayerAdded)
	{
		auto curGameState = GetWorld()->GetGameState<AOnlineGameState>();
		for (APlayerState* PlayerState : ConnectedPlayerArray)
		{
			if (PlayerState != NewPlayerState)
			{
				curGameState->OnPlayerJoined.Broadcast(PlayerState);
			}
		}
		isInitializedNewPlayerAdded = true;
	}
}

void AOnlineGameState::AddPlayerPoint(int PlayerID, int Score)
{
	PlayerScoreBoard.FindOrAdd(PlayerID) += Score;
	FScoreData ScoreData;
	ScoreData.PlayerScores = PlayerScoreBoard;
	ScoreData.TeamScores = TeamScoreBoard;
	OnScoreUpdated.Broadcast(ScoreData);
}

void AOnlineGameState::AddTeamPoint(int TeamID, int Score)
{
	TeamScoreBoard.FindOrAdd(TeamID) += Score;
	FScoreData ScoreData;
	ScoreData.PlayerScores = PlayerScoreBoard;
	ScoreData.TeamScores = TeamScoreBoard;
	OnScoreUpdated.Broadcast(ScoreData);
}

void AOnlineGameState::BroadcastPendingPlayerJoins()
{
	if (HasPlayerJoinedDelegates() && PendingJoinedPlayers.Num() > 0)
	{
		// 广播所有缓存的玩家加入事件
		for (APlayerState* PlayerState : PendingJoinedPlayers)
		{
			OnPlayerJoined.Broadcast(PlayerState);
		}

		// 清空缓存
		PendingJoinedPlayers.Empty();

		// 停止定时器，因为已经不需要再检查了
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

void AOnlineGameState::OnMatchEnded(int winnerPlayerID, int winnerTeamID)
{
	WinnerID = winnerPlayerID;
	WinningTeamID = winnerTeamID;
	NetMulticast_MatchEnded(winnerPlayerID, winnerTeamID);
}

void AOnlineGameState::NetMulticast_MatchEnded_Implementation(int winnerPlayerID, int winnerTeamID)
{
	//设置输入模式仅UI
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
		//打印胜利者ID
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("WinnerID: %d"), winnerPlayerID));
	}
}
