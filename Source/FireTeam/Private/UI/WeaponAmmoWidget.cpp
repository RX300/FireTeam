// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponAmmoWidget.h"
#include "Characters/MyFTCharacter.h"
#include "Weapons/WeaponActor.h"
#include "Components/TextBlock.h"
UWeaponAmmoWidget::UWeaponAmmoWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{

}

void UWeaponAmmoWidget::FetchWeaponReference()
{
	//��ȡ��ǰ���Character
	auto FpMesh = Cast<AMyFTCharacter>(GetOwningPlayerPawn())->GetMesh1P();
	//��FpMesh�ϻ�ȡ����ΪFP_Gun��child actor���FP_Gun
	TArray<USceneComponent*> ChildrenComs;
	FpMesh->GetChildrenComponents(false, ChildrenComs);
	AActor* FpGun = nullptr;
	for(auto & child:ChildrenComs)
	{
		if (child->GetName() == "FP_Gun")
		{
			//��ȡ��FP_Gun��child actor���
			FpGun = Cast<UChildActorComponent>(child)->GetChildActor();
		}
	}
	//��FpGunת����WeaponActor
	WeaponActor = Cast<AWeaponActor>(FpGun);
	if(!WeaponActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("WeaponActor is nullptr"));
	}
}

void UWeaponAmmoWidget::UpdateAmmoText()
{
	if(WeaponActor)
	{
		//��ȡ��ǰ��ҩ����
		int32 CurrentAmmo = WeaponActor->CurrentAmmo;
		//��ȡ���ҩ����
		int32 MaxAmmo = WeaponActor->MaxAmmo;
		//���õ�ǰ��ҩ����
		CurrentAmmoText->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
		//�������ҩ����
		MaxAmmoText->SetText(FText::FromString(FString::FromInt(MaxAmmo)));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("WeaponActor is nullptr"));
	}
}

void UWeaponAmmoWidget::InitWeaponAmmoWidget()
{
	FetchWeaponReference();
	if (!WeaponActor)
	{
		// ÿ�� 0.5 �볢��һ�Σ�ֱ���ɹ�
		GetWorld()->GetTimerManager().SetTimer(
			timerHandle,
			FTimerDelegate::CreateLambda([this]() {
				FetchWeaponReference();
				UpdateAmmoText();
				if (WeaponActor)
				{
					//clear timer
					GetWorld()->GetTimerManager().ClearTimer(timerHandle);
					WeaponActor->OnUpdateAmmo.AddDynamic(this, &UWeaponAmmoWidget::UpdateAmmoText);
				}
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Retry Fetch WeaponActor"));
			}),
			0.5f,
			true
		);
	}
	else
	{
		UpdateAmmoText();
		WeaponActor->OnUpdateAmmo.AddDynamic(this, &UWeaponAmmoWidget::UpdateAmmoText);
	}
}
