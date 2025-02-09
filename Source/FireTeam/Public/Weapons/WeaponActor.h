// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CombatInterface.h"
#include "WeaponActor.generated.h"

// 声明带三个参数的事件
// 声明动态多播委托（支持蓝图和网络复制）
// 蓝图里的事件分发器实际上就是一个动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FServeShootingEventDispatcher,
	FVector, Origin,
	FVector, Direction,
	APlayerController*, Controller);

class AMyFTCharacter;

UCLASS()
class FIRETEAM_API AWeaponActor : public AActor,public ICombatInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 重写 GetLifetimeReplicatedProps 以设置复制属性
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ICombatInterface interface
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PrimaryFire(bool isFiring) override;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Reload() override;
	// End of ICombatInterface interface

	// 定义事件
	// Shoot相关
	UPROPERTY(BlueprintAssignable, Category = "CustomEvents")
	FServeShootingEventDispatcher ServeShootingEDispatcher;
	// 服务器实现事件的函数，函数名前面必须以Server开头
	UFUNCTION(Server, Reliable,BlueprintCallable,Category = "Combat")
	void Server_Shoot(FVector Origin, FVector Direction, APlayerController* Controller);
	// 声明多播 RPC 函数（名称必须以 Multicast_ 开头）
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Combat")
	void Multicast_FireBullet(FVector Origin, FVector Direction, const FHitResult& HitResult);
	// Reload相关
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void Server_Reload();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Combat")
	void Multicast_Reload();

public:
	//蓝图可编辑的骨骼网格体组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	//场景组件，表示子弹发射的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Weapon")
	TObjectPtr<USceneComponent> BulletSceneComponent;
	// Shoot动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UAnimMontage> FP_FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UAnimMontage> TP_FireAnimation;
	// Reload动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UAnimMontage> FP_ReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UAnimMontage> TP_ReloadAnimation;

private:
	// 用于存储武器的拥有者
	TObjectPtr<AMyFTCharacter> WeaponOwner;
};