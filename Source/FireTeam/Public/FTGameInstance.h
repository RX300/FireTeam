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
	//指向在线子系统的接口指针
	//头文件不能直接用前向声明的IOnlineSessionPtr(因为它是一个别名)，要么加头文件要么按下面这种写法
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> OnlineSessionInterface;
	TSharedPtr<FOnlineSessionSearch> SearchSettings;
private:
	//创建会话完成后的回调函数
	//比如在线子系统向steam服务创建一个会话，steam创建成功后，会回传信息给在线子系统
	//在线子系统再调用这个回调函数
	//另外，FOnCreateSessionCompleteDelegate也是个别名，这里采用加头文件的方式
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
};
