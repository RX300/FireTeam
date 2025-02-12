// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Characters/MyFTCharacter.h"

void UHealthBarWidget::FetchCharacterReference()
{
    // 获取当前玩家Character
    CurCharacter = Cast<AMyFTCharacter>(GetOwningPlayerPawn());
    if (!CurCharacter)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Character is nullptr"));
    }
}

void UHealthBarWidget::UpdateHealthBar(float CurrentHealth)
{
    if (CurCharacter)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Update Health Bar"));
        // 获取最大生命值
        float MaxHealth = CurCharacter->MaxHealth;
        // 设置进度条的百分比
        HealthBar->SetPercent(CurrentHealth / MaxHealth);
        //打印百分比
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), CurrentHealth / MaxHealth));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("CurrentHealth: %f"), CurrentHealth));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("MaxHealth: %f"), MaxHealth));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Character is nullptr"));
    }
}

void UHealthBarWidget::InitHealthBarWidget()
{
    FetchCharacterReference();
    if (!CurCharacter)
    {
        // 每隔 0.5 秒尝试一次，直到成功
        GetWorld()->GetTimerManager().SetTimer(
            timerHandle,
            FTimerDelegate::CreateLambda([this]() {
                FetchCharacterReference();
                if (CurCharacter)
                {
                    // 清除定时器
                    GetWorld()->GetTimerManager().ClearTimer(timerHandle);
                    float CurrentHealth = CurCharacter->CurrentHealth;
                    UpdateHealthBar(CurrentHealth);
                    CurCharacter->OnHealthChanged.AddDynamic(this, &UHealthBarWidget::UpdateHealthBar);
                }
                GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Retry Fetch Character"));
                }),
            0.5f,
            true
        );
    }
    else
    {
        float CurrentHealth = CurCharacter->CurrentHealth;
        UpdateHealthBar(CurrentHealth);
        CurCharacter->OnHealthChanged.AddDynamic(this, &UHealthBarWidget::UpdateHealthBar);
    }
}
