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
	//获取当前玩家Character
	auto FpMesh = Cast<AMyFTCharacter>(GetOwningPlayerPawn())->GetMesh1P();
	//从FpMesh上获取到名为FP_Gun的child actor组件FP_Gun
	TArray<USceneComponent*> ChildrenComs;
	FpMesh->GetChildrenComponents(false, ChildrenComs);
	AActor* FpGun = nullptr;
	for(auto & child:ChildrenComs)
	{
		if (child->GetName() == "FP_Gun")
		{
			//获取到FP_Gun的child actor组件
			FpGun = Cast<UChildActorComponent>(child)->GetChildActor();
		}
	}
	//将FpGun转换成WeaponActor
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
		//获取当前弹药数量
		int32 CurrentAmmo = WeaponActor->CurrentAmmo;
		//获取最大弹药数量
		int32 MaxAmmo = WeaponActor->MaxAmmo;
		//设置当前弹药数量
		CurrentAmmoText->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
		//设置最大弹药数量
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
		// 每隔 0.5 秒尝试一次，直到成功
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
