// Fill out your copyright notice in the Description page of Project Settings.


#include "FTGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

UFTGameInstance::UFTGameInstance()
{
    IOnlineSubsystem* OnlineSubSystem = IOnlineSubsystem::Get();
    OnlineSessionInterface = OnlineSubSystem->GetSessionInterface();
 //   if(OnlineSubSystem)
	//{
	//	OnlineSessionInterface = OnlineSubSystem->GetSessionInterface();
 //       //打印当前在线子系统的名称
 //       GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
 //           FString::Printf(TEXT("Found Online Subsysetm %s"), *OnlineSubSystem->GetSubsystemName().ToString()));
	//}
 //   else
 //   {
 //       GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("Failed to get online subsystem."));
 //   }
    OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UFTGameInstance::OnCreateSessionComplete);
    OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UFTGameInstance::OnFindSessionsComplete);
}

// 快速匹配，直接加入一个匹配(会话)
void UFTGameInstance::QuickMatch()
{
 //   if(!OnlineSessionInterface.IsValid())
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid."));
	//	return;
	//}
 //   // 配置搜索设置
 //   TSharedPtr<FOnlineSessionSearch> SearchSettings = MakeShareable(new FOnlineSessionSearch());
 //   SearchSettings->MaxSearchResults = 1;  // 快速匹配，只寻找一个会话
 //   SearchSettings->bIsLanQuery = true;    // 如果是本地网络游戏，可以设置为true

 //   // 查找会话
 //   OnlineSessionInterface->FindSessions(0, SearchSettings.ToSharedRef());

 //   // 如果找到了匹配的会话，加入
 //   if (SearchSettings->SearchResults.Num() > 0)
 //   {
 //       // 选择第一个匹配的会话
 //       const FOnlineSessionSearchResult& SearchResult = SearchSettings->SearchResults[0];

 //       // 获取本地玩家唯一 ID
 //       const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();
 //       auto uniqueNetID = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();

 //       // 尝试加入会话
 //       bool isJoinSuccess = OnlineSessionInterface->JoinSession(*uniqueNetID, TEXT("FireTeam"), SearchResult);
 //       if (isJoinSuccess)
 //       {
 //           // 加入成功后，加载关卡
 //           UGameplayStatics::OpenLevel(GetWorld(), TEXT("LobbyMenu"));
 //       }
 //       else
 //       {
 //           UE_LOG(LogTemp, Warning, TEXT("QuickMatch: Failed to join session."));
 //       }
 //   }
 //   else
 //   {
 //       UE_LOG(LogTemp, Warning, TEXT("QuickMatch: No session found."));
 //   }
}

void UFTGameInstance::HostMatch()
{
    if (!OnlineSessionInterface.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid."));
        return;
    }
    bool isCreateSession = false;
    auto SessionName = TEXT("FireTeam");
    auto ExistingSession = OnlineSessionInterface->GetNamedSession(SessionName);
    if (ExistingSession != nullptr)
    {
		// 如果已经存在会话，先销毁
		isCreateSession = OnlineSessionInterface->DestroySession(SessionName);
	}
    // 添加创建会话完成的委托
    OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
    // 配置会话设置
    TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->bIsLANMatch = true;			// 会话设置：不创建 LAN 连接
    SessionSettings->NumPublicConnections = 8;		// 会话设置：设置最大公共连接数为 4
    SessionSettings->bAllowJoinInProgress = true;	// 会话设置：在会话运行时允许其他玩家加入
    SessionSettings->bIsDedicated = true;			// 会话设置：是专用服务器/host主机
    SessionSettings->bAllowJoinViaPresence = true;	// 会话设置：Steam 使用 Presence 搜索会话所在地区，确保连接正常工作
    SessionSettings->bShouldAdvertise = true;		// 会话设置：允许 Steam 发布会话
    //SessionSettings->bUsesPresence = true;			// 会话设置：允许显示用户 Presence 信息
    SessionSettings->bUseLobbiesIfAvailable = true;	// （视频中未提及）会话设置：优先选择 Lobby API（Steam 支持 Lobby API）

    //通过LocalPlayer获取UniqueNetId
    // 获取本地玩家
    const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();
    auto uniqueNetID = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
    // 创建会话
    isCreateSession = OnlineSessionInterface->CreateSession(*uniqueNetID, SessionName, *SessionSettings);
    if(isCreateSession)
	{
		//open level
        //?listen 是一个特殊的 URL 参数，用于在启动新关卡时指定该实例作为 Listen Server
        //启动关卡 LobbyMenu，并使当前的客户端充当 Listen Server。
        UGameplayStatics::OpenLevel(GetWorld(), TEXT("LobbyMenu"), true, "?listen");
	}
}

//查找匹配，返回最多10个匹配(会话)结果
void UFTGameInstance::FindMatch()
{
    if (!OnlineSessionInterface.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid."));
        return;
    }
    OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
    // 配置搜索设置
    if(SearchSettings.IsValid())
	{
		SearchSettings.Reset();
	}
    SearchSettings = MakeShareable(new FOnlineSessionSearch());
    SearchSettings->MaxSearchResults = 10000;  // 返回最多10个会话
    SearchSettings->bIsLanQuery = true;     // 本地网络查询
    //SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
    // 查找会话
    const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();
    auto uniqueNetID = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
    OnlineSessionInterface->FindSessions(*uniqueNetID, SearchSettings.ToSharedRef());
}

void UFTGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
           FString::Printf(TEXT("Create Session: %s"),*SessionName.ToString()));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Create session failed."));
    }
}

void UFTGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    if(!bWasSuccessful)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Find session failed."));
    if(SearchSettings->SearchResults.Num() == 0)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No session found."));
    for(const FOnlineSessionSearchResult& SearchResult : SearchSettings->SearchResults)
	{
        FString IdStr = SearchResult.GetSessionIdStr();
        FString UserName = SearchResult.Session.OwningUserName;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString::Printf(TEXT("Session: %s,Id:%d User:%d"), *SearchResult.GetSessionIdStr(),
                *IdStr, *UserName));
	}
}

