// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponAmmoWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class AWeaponActor;
UCLASS()
class FIRETEAM_API UWeaponAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UWeaponAmmoWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable)
	void FetchWeaponReference();
	UFUNCTION(BlueprintCallable)
	void UpdateAmmoText();
	UFUNCTION(BlueprintCallable)
	void InitWeaponAmmoWidget();
public:
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AWeaponActor> WeaponActor;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentAmmoText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxAmmoText;
private:
	FTimerHandle timerHandle;
};
