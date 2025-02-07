// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponActor.h"
#include "FireTeamCharacter.h"
#include "Net/UnrealNetwork.h" // 使用DOREPLIFETIME必须包含此头文件
#include "Kismet/GameplayStatics.h"
#include "Public/Characters/MyFTCharacter.h"
// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//初始化武器的骨骼网格体组件和子弹发射的位置(场景组件)，并且把场景组件挂载到骨骼网格体组件下
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	BulletSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSceneComponent"));
	BulletSceneComponent->SetupAttachment(WeaponMesh);
	//设置类为Replicated
	bReplicates = true;

}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	//找到武器Actor的父Actor，并且转换为MyFTCharacter类型
	AActor* ParentActor = GetAttachParentActor();
	if (ParentActor)
	{
		AMyFTCharacter* MyCharacter = Cast<AMyFTCharacter>(ParentActor);
		if (MyCharacter)
		{
			//设置武器的拥有者为这个MyFTCharacter
			SetOwner(MyCharacter);
			WeaponOwner = MyCharacter;
			WeaponOwner->SetReplicates(true);
			WeaponOwner->SetReplicateMovement(true);
			WeaponOwner->OnCustomEvent();
		}
		else
		{
			//如果没有找到MyFTCharacter，那么就打印一个错误信息
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("WeaponActor's ParentActor is not MyFTCharacter"));
		}
	}
	//绑定事件
	ServeShootingEDispatcher.AddDynamic(this, &AWeaponActor::Shoot);
}

void AWeaponActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//设置可复制的属性
	DOREPLIFETIME(AWeaponActor, WeaponMesh);
	DOREPLIFETIME(AWeaponActor, BulletSceneComponent);
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor::PrimaryFire(bool isFiring)
{
	if (isFiring)
	{
		//if (GetLocalRole() == ROLE_Authority) {//仅在服务器上调用
		auto MyCharacter = Cast<AFireTeamCharacter>(GetOwner());
		auto curController = Cast<APlayerController>(MyCharacter->GetController());
		ServeShootingEDispatcher.Broadcast(BulletSceneComponent->GetComponentLocation(), BulletSceneComponent->GetForwardVector(), curController);
	}
}

void AWeaponActor::Multicast_FireBullet_Implementation(FVector Origin, FVector Direction, const FHitResult& HitResult)
{
	
	//判断当前拥有者是本地玩家还是远程玩家
	if(WeaponOwner->IsLocallyControlled())
	{
		//播放本地玩家的第一人称动画
		auto FP_Mesh = WeaponOwner->GetMesh1P();
		//play animation_montage
		//Get Animation Montage From Text
		//UAnimMontage* FireMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("/Game/FirstPersonArms/Animations/FP_Rifle_Shoot_Montage")));
		FP_Mesh->GetAnimInstance()->Montage_Play(FP_FireAnimation, 1.0f);
	}
	else
	{
		//播放其他玩家的第三人称动画
		auto TP_Mesh=WeaponOwner->TP_Mesh;
		//UAnimMontage* FireMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("/Game/_Game/Animations/CustomTPFire_Rifle_Hip_Montage")));
		TP_Mesh->GetAnimInstance()->Montage_Play(TP_FireAnimation, 1.0f);
	}
}

void AWeaponActor::Shoot_Implementation(FVector Origin, FVector Direction, APlayerController* Controller)
{
	// 检查 Controller 有效性
	if (!Controller || !Controller->GetPawn()) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Controller or Pawn!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid Controller or Pawn!"));
		return;
	}
	// 定义线性追踪的起始和结束位置
	FVector Start = Origin; // 起始点
	FVector End = Origin + Direction * 10000; // 结束点，长度为10000单位
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shoot"));
	// 执行线性追踪
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true);
	TraceParams.AddIgnoredActor(Controller->GetPawn());
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
	{
		// 如果有碰撞，您可以在这里处理碰撞结果
		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), 20.0f, Direction, HitResult, Controller, this, UDamageType::StaticClass());
	}

	// 绘制调试线，持续时间为5秒
	DrawDebugLine(GetWorld(), Start, HitResult.bBlockingHit?HitResult.Location:End, FColor::Green, false, 5.0f, 0, 1.0f);
	// 调用多播函数
	Multicast_FireBullet(Origin, Direction, HitResult);
}