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
 //       //��ӡ��ǰ������ϵͳ������
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

// ����ƥ�䣬ֱ�Ӽ���һ��ƥ��(�Ự)
void UFTGameInstance::QuickMatch()
{
 //   if(!OnlineSessionInterface.IsValid())
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid."));
	//	return;
	//}
 //   // ������������
 //   TSharedPtr<FOnlineSessionSearch> SearchSettings = MakeShareable(new FOnlineSessionSearch());
 //   SearchSettings->MaxSearchResults = 1;  // ����ƥ�䣬ֻѰ��һ���Ự
 //   SearchSettings->bIsLanQuery = true;    // ����Ǳ���������Ϸ����������Ϊtrue

 //   // ���һỰ
 //   OnlineSessionInterface->FindSessions(0, SearchSettings.ToSharedRef());

 //   // ����ҵ���ƥ��ĻỰ������
 //   if (SearchSettings->SearchResults.Num() > 0)
 //   {
 //       // ѡ���һ��ƥ��ĻỰ
 //       const FOnlineSessionSearchResult& SearchResult = SearchSettings->SearchResults[0];

 //       // ��ȡ�������Ψһ ID
 //       const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();
 //       auto uniqueNetID = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();

 //       // ���Լ���Ự
 //       bool isJoinSuccess = OnlineSessionInterface->JoinSession(*uniqueNetID, TEXT("FireTeam"), SearchResult);
 //       if (isJoinSuccess)
 //       {
 //           // ����ɹ��󣬼��عؿ�
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
		// ����Ѿ����ڻỰ��������
		isCreateSession = OnlineSessionInterface->DestroySession(SessionName);
	}
    // ��Ӵ����Ự��ɵ�ί��
    OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
    // ���ûỰ����
    TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->bIsLANMatch = true;			// �Ự���ã������� LAN ����
    SessionSettings->NumPublicConnections = 8;		// �Ự���ã�������󹫹�������Ϊ 4
    SessionSettings->bAllowJoinInProgress = true;	// �Ự���ã��ڻỰ����ʱ����������Ҽ���
    SessionSettings->bIsDedicated = true;			// �Ự���ã���ר�÷�����/host����
    SessionSettings->bAllowJoinViaPresence = true;	// �Ự���ã�Steam ʹ�� Presence �����Ự���ڵ�����ȷ��������������
    SessionSettings->bShouldAdvertise = true;		// �Ự���ã����� Steam �����Ự
    //SessionSettings->bUsesPresence = true;			// �Ự���ã�������ʾ�û� Presence ��Ϣ
    SessionSettings->bUseLobbiesIfAvailable = true;	// ����Ƶ��δ�ἰ���Ự���ã�����ѡ�� Lobby API��Steam ֧�� Lobby API��

    //ͨ��LocalPlayer��ȡUniqueNetId
    // ��ȡ�������
    const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();
    auto uniqueNetID = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
    // �����Ự
    isCreateSession = OnlineSessionInterface->CreateSession(*uniqueNetID, SessionName, *SessionSettings);
    if(isCreateSession)
	{
		//open level
        //?listen ��һ������� URL �����������������¹ؿ�ʱָ����ʵ����Ϊ Listen Server
        //�����ؿ� LobbyMenu����ʹ��ǰ�Ŀͻ��˳䵱 Listen Server��
        UGameplayStatics::OpenLevel(GetWorld(), TEXT("LobbyMenu"), true, "?listen");
	}
}

//����ƥ�䣬�������10��ƥ��(�Ự)���
void UFTGameInstance::FindMatch()
{
    if (!OnlineSessionInterface.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid."));
        return;
    }
    OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
    // ������������
    if(SearchSettings.IsValid())
	{
		SearchSettings.Reset();
	}
    SearchSettings = MakeShareable(new FOnlineSessionSearch());
    SearchSettings->MaxSearchResults = 10000;  // �������10���Ự
    SearchSettings->bIsLanQuery = true;     // ���������ѯ
    //SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
    // ���һỰ
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

