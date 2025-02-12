// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuUIManger.generated.h"

class UUserWidget;
class UWidgetSwitcher;
class AHUD;
UCLASS()
class FIRETEAM_API AMainMenuUIManger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainMenuUIManger();
	
	//MainMenuTarBar按钮点击时的事件函数
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnMainMenuTarBarClicked(int index);
	//Init MainMenuHUD
	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitMainMenuHUD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> MainMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> LobbyWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> MainMenuTarBarWidge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> MainMenuPlayScreenWidget;
	//控件切换器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<AHUD> MainMenuHUD;
private:
	FTimerHandle timerHandle;
};
