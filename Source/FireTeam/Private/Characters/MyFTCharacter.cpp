// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyFTCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h" // 网络编程必须包含这个头文件
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

void AMyFTCharacter::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("My FT Character"));
	
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
}

void AMyFTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerComponent)
{
	Super::SetupPlayerInputComponent(PlayerComponent);
}
