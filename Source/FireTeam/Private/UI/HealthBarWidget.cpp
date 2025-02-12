// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Characters/MyFTCharacter.h"

void UHealthBarWidget::FetchCharacterReference()
{
    // ��ȡ��ǰ���Character
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
        // ��ȡ�������ֵ
        float MaxHealth = CurCharacter->MaxHealth;
        // ���ý������İٷֱ�
        HealthBar->SetPercent(CurrentHealth / MaxHealth);
        //��ӡ�ٷֱ�
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
        // ÿ�� 0.5 �볢��һ�Σ�ֱ���ɹ�
        GetWorld()->GetTimerManager().SetTimer(
            timerHandle,
            FTimerDelegate::CreateLambda([this]() {
                FetchCharacterReference();
                if (CurCharacter)
                {
                    // �����ʱ��
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
