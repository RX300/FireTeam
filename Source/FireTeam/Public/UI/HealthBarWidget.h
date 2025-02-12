// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
class AMyFTCharacter;
UCLASS()
class FIRETEAM_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void FetchCharacterReference();
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float CurrentHealth);
	UFUNCTION(BlueprintCallable)
	void InitHealthBarWidget();
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;
	UPROPERTY(BlueprintReadWrite, Category = "Character Info")
	TObjectPtr<AMyFTCharacter> CurCharacter;
private:
	FTimerHandle timerHandle;
	
};
