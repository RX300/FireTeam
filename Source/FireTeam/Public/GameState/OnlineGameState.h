// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OnlineGameState.generated.h"

USTRUCT(BlueprintType)
struct FScoreData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TMap<int32, int32> PlayerScores;

	UPROPERTY(BlueprintReadWrite)
	TMap<int32, int32> TeamScores;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam
(FOnScoreUpdated, 
const FScoreData&, ScoreData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerJoined, 
APlayerState*, NewPlayerState);

/**
 * 
 */
class UKillAnnouncementWidget;

UCLASS()
class FIRETEAM_API AOnlineGameState : public AGameState
{
	GENERATED_BODY()
public:
	// 在PostInitializeComponents中调用，确保所有组件初始化完成
	virtual void PostInitializeComponents() override;
	//GetLifetimeReplicatedProps
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UFUNCTION(Server,Reliable, BlueprintCallable, Category = "Custom Events")
	void Server_AnnounceKill(APlayerState* playerstate,const FString& Victim);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Custom Events")
	virtual void NetMulticast_KillAnnounced(APlayerState* playerstate, const FString& Victim);
	//这里实际上只需要它在服务器上调用，但是因为这个函数是在GameMode里调用的，所以不需要在这里声明为Server
	//GameMode里调用，GameMode仅仅只在服务器端存在
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void NewPlayerJoined(APlayerController* NewPlayer);
	// 添加一个网络多播函数，通知所有客户端有新玩家加入
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Custom Events")
	void Multicast_NewPlayerJoined(APlayerState* NewPlayerState);
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void AddPlayerPoint(int PlayerID, int Score);
	UPROPERTY(BlueprintAssignable, Category = "Custom Events")
	FOnScoreUpdated OnScoreUpdated;
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void AddTeamPoint(int TeamID, int Score);
	UPROPERTY(BlueprintAssignable, Category = "Custom Events")
	FOnPlayerJoined OnPlayerJoined;
	// 延迟广播玩家加入事件的方法
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void BroadcastPendingPlayerJoins();
public:
	//KillAnnouncementWidgetClass
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UKillAnnouncementWidget> KillAnnouncementWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TMap<int32, int32> PlayerScoreBoard;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TMap<int32, int32> TeamScoreBoard;
	//因为GameMode只存在于服务器端，所以用GameState来同步数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 WinThreshold=10;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game Info")
	TArray<TObjectPtr<APlayerState>> ConnectedPlayerArray;
private:
	// 缓存已加入但未广播的玩家
	TArray<TObjectPtr<APlayerState>> PendingJoinedPlayers;

	// 检查委托是否有绑定
	bool HasPlayerJoinedDelegates() const
	{
		return OnPlayerJoined.IsBound();
	}
	FTimerHandle TimerHandle;
	bool isInitializedNewPlayerAdded = false;
};
