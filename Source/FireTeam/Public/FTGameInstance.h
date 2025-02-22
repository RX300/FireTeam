// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "FTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRETEAM_API UFTGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFTGameInstance();
	//virtual void Init() override;
	//virtual void Shutdown() override;
	//virtual void StartGameInstance() override;
	//virtual void OnStart() override;
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void QuickMatch();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HostMatch();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void FindMatch();
protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
public:
	//ָ��������ϵͳ�Ľӿ�ָ��
	//ͷ�ļ�����ֱ����ǰ��������IOnlineSessionPtr(��Ϊ����һ������)��Ҫô��ͷ�ļ�Ҫô����������д��
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> OnlineSessionInterface;
	TSharedPtr<FOnlineSessionSearch> SearchSettings;
private:
	//�����Ự��ɺ�Ļص�����
	//����������ϵͳ��steam���񴴽�һ���Ự��steam�����ɹ��󣬻�ش���Ϣ��������ϵͳ
	//������ϵͳ�ٵ�������ص�����
	//���⣬FOnCreateSessionCompleteDelegateҲ�Ǹ�������������ü�ͷ�ļ��ķ�ʽ
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
};
