// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyFTCharacter.h"
#include "Controller/MyPlayerController.h"
#include "UI/InGameHUD.h"
#include "UI/HealthBarWidget.h"
#include "UI/WeaponAmmoWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h" // 网络编程必须包含这个头文件
#include "PlayerState/OnlinePlayerState.h"

AMyFTCharacter::AMyFTCharacter():Super()
{
	//创建一个第三人称骨骼网格体组件并且挂载到根组件上
	ThirdPersonLegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonLegMesh"));
	ThirdPersonLegMesh->SetupAttachment(GetRootComponent());
	FirstPersonShadowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonShadowMesh"));
	FirstPersonShadowMesh->SetupAttachment(GetRootComponent());
	//设置仅拥有者可见
	ThirdPersonLegMesh->SetOnlyOwnerSee(true);
	FirstPersonShadowMesh->SetOnlyOwnerSee(true);
	GetMesh1P()->SetOnlyOwnerSee(true);
	// 启用网络同步
	bReplicates = true;
	SetReplicateMovement(true); // 如果需要同步移动
}

void AMyFTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//设置CharacterRotation属性在客户端和服务器端同步
	DOREPLIFETIME(AMyFTCharacter, CharacterControllerRotation);
	DOREPLIFETIME(AMyFTCharacter, MaxHealth);
	DOREPLIFETIME(AMyFTCharacter, CurrentHealth);
	DOREPLIFETIME(AMyFTCharacter, isDead);
}

float AMyFTCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Multicast_OnHealthChanged(ActualDamage);
	return ActualDamage;
}

void AMyFTCharacter::FromServerSetCharacterRotation() 
{
	//先判断是否是服务器端
	if (GetLocalRole() == ROLE_Authority)
	{
		//如果是服务器端，直接设置控制器的旋转量
		CharacterControllerRotation = GetControlRotation();
	}
}

void AMyFTCharacter::SetAimOffset()
{
	//获取当前Actor的旋转(不是控制器的旋转量)
	FRotator CurActorRotation = GetActorRotation();
	//设置新的旋转量(控制器旋转和Actor旋转的差值)
	FRotator NewRotation = CharacterControllerRotation - CurActorRotation;
	//将这个新的旋转量插值设置Yaw和Pitch
	Yaw = FMath::FInterpTo(NewRotation.Yaw, NewRotation.Yaw, GetWorld()->GetDeltaSeconds(), 5.0f);
	Pitch = FMath::FInterpTo(NewRotation.Pitch, NewRotation.Pitch, GetWorld()->GetDeltaSeconds(), 5.0f);
	Yaw = FMath::ClampAngle(Yaw, -90.0f, 90.0f);
	Pitch = FMath::ClampAngle(Pitch, -90.0f, 90.0f);
}

void AMyFTCharacter::SetThirdPersonMesh(USkeletalMeshComponent* ThirdMesh)
{
	TP_Mesh = ThirdMesh;
}

void AMyFTCharacter::Multicast_OnHealthChanged_Implementation(float DamageCount)
{
	CurrentHealth -= DamageCount;
	if (CurrentHealth <= 0 && !isDead)
	{
		isDead = true;
		Multicast_OnDeath();
		Client_OnDeath();

		// 调试输出LastHitBy
		if (LastHitBy.Get())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				FString::Printf(TEXT("LastHitBy : %s"), *LastHitBy.Get()->GetName()));
			auto killerChar = Cast<AMyFTCharacter>(LastHitBy.Get()->GetCharacter());
			auto killState = killerChar->GetPlayerState<AOnlinePlayerState>();
			killState->GainKills(GetPlayerState<AOnlinePlayerState>()->GetPlayerName());
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LastHitBy still null in TakeDamage"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Health: %f"), CurrentHealth));
	OnHealthChanged.Broadcast(CurrentHealth);
}

void AMyFTCharacter::Multicast_OnDeath_Implementation()
{
	//启用TP_Mesh的物理模拟
	TP_Mesh->SetSimulatePhysics(true);
	//设置TP_Mesh的碰撞类型为纯物理(不查询碰撞)
	TP_Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//获取碰撞胶囊体
	UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(GetCapsuleComponent());
	//设置碰撞胶囊体的碰撞类型为无碰撞
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//禁用角色移动组件
	GetCharacterMovement()->DisableMovement();
	CurrentHealth = 0.0f;
	//设置武器的场景组件在游戏中不可见
	TP_Gun->SetHiddenInGame(true);
	FP_Gun->SetHiddenInGame(true);
}

void AMyFTCharacter::Client_OnDeath_Implementation()
{
	//设置ThirdPersonLegMesh为拥有者不可见
	ThirdPersonLegMesh->SetOwnerNoSee(true);
	GetMesh1P()->SetOwnerNoSee(true);
	//设置第三人称Mesh可见
	TP_Mesh->SetOwnerNoSee(false);
	//set camera
	GetFirstPersonCameraComponent()->SetActive(false);
	TPDeath_Camera->SetActive(true);
	//set cast shadow off
	FirstPersonShadowMesh->SetCastShadow(false);
	auto shadowGun = FindComponentByTag<USkeletalMeshComponent>(TEXT("Shadow_Gun"));
	shadowGun->SetCastShadow(false);
	//设置定时器，5s后重新生成角色
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyFTCharacter::RespawnRequest, 5.0f, false);

}

void AMyFTCharacter::RespawnRequest()
{
	//Get Player Controller
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	if (PlayerController)
	{
		//调用PlayerController的Server_ReSpawnRequest方法
		PlayerController->Server_ReSpawnRequest();
	}
}

void AMyFTCharacter::RefreshHUD()
{
	//获取玩家控制器
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	AInGameHUD* InGameHud = nullptr;
	if (PlayerController)
		InGameHud = Cast<AInGameHUD>(PlayerController->GetHUD());
	FTimerHandle TimerHandle;
	//如果玩家控制器为空，使用定时器每隔0.1s获取一次PlayerController，循环直到获取到PlayerController，使用Lambda表达式
	if (!PlayerController||!InGameHud)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &TimerHandle,&InGameHud]() {
			AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
			if (PlayerController)
				InGameHud = Cast<AInGameHUD>(PlayerController->GetHUD());
			if (PlayerController&&InGameHud)
			{
				// 清除定时器
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(InGameHud->InGameHUDWidget->GetWidgetFromName(TEXT("BP_HealthBarWidget")));
				HealthBarWidget->InitHealthBarWidget();
				UWeaponAmmoWidget* WeaponAmmoWidget = Cast<UWeaponAmmoWidget>(InGameHud->InGameHUDWidget->GetWidgetFromName(TEXT("BP_AmmoWidget")));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("RefreshWeapomHUD"));
				WeaponAmmoWidget->InitWeaponAmmoWidget();
			}
			}, 0.2f, true);
		return;
	}
	else
	{
		//在InGameHUDWidget中查找UHealthBarWidget类
		UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(InGameHud->InGameHUDWidget->GetWidgetFromName(TEXT("BP_HealthBarWidget")));
		HealthBarWidget->InitHealthBarWidget();
		UWeaponAmmoWidget* WeaponAmmoWidget = Cast<UWeaponAmmoWidget>(InGameHud->InGameHUDWidget->GetWidgetFromName(TEXT("BP_AmmoWidget")));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("RefreshWeapomHUD"));
		WeaponAmmoWidget->InitWeaponAmmoWidget();
	}
}

void AMyFTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//按名称隐藏骨骼spine_01
	ThirdPersonLegMesh->HideBoneByName(TEXT("spine_01"), EPhysBodyOp::PBO_None);
	//把ShadowMesh的骨骼网格体资产设置成和ThirdPersonMesh一样，并且transform参数也一样
	FirstPersonShadowMesh->SetSkeletalMesh(ThirdPersonLegMesh->SkeletalMesh);
	// 设置 FirstPersonShadowMesh 的相对位置、旋转和缩放
	FirstPersonShadowMesh->SetRelativeLocation(ThirdPersonLegMesh->GetRelativeLocation());
	FirstPersonShadowMesh->SetRelativeRotation(ThirdPersonLegMesh->GetRelativeRotation());
	FirstPersonShadowMesh->SetRelativeScale3D(ThirdPersonLegMesh->GetRelativeScale3D());
	//设置ShadowMesh的渲染在主通道不渲染
	FirstPersonShadowMesh->SetRenderInMainPass(false);
	//设置ShadowMesh的动画蓝图和ThirdPersonMesh一样
	FirstPersonShadowMesh->SetAnimInstanceClass(ThirdPersonLegMesh->GetAnimInstance()->GetClass());

	//查找名为Serve_TP_Gun的子Actor组件，赋值给TP_Gun
	TP_Gun = FindComponentByTag<UChildActorComponent>(TEXT("TP_Gun"));
	FP_Gun = FindComponentByTag<UChildActorComponent>(TEXT("FP_Gun"));
}

void AMyFTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerComponent)
{
	Super::SetupPlayerInputComponent(PlayerComponent);
}
