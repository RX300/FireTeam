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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateAmmoDelegate);

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
	UPROPERTY(BlueprintAssignable, Category = "CustomEventsDispatcher")
	FServeShootingEventDispatcher ServeShootingEDispatcher;
	FUpdateAmmoDelegate OnUpdateAmmo;
	// 服务器实现事件的函数，函数名前面必须以Server开头
	UFUNCTION(Server, Reliable,BlueprintCallable,Category = "Combat")
	void Server_Shoot(FVector Origin, FVector Direction, APlayerController* Controller);
	// 声明多播 RPC 函数（名称必须以 Multicast_ 开头）
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Combat")
	void Multicast_Shoot(FVector Origin, FVector Direction, const FHitResult& HitResult);
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Combat")
	void Client_Shoot(FVector Origin, FVector Direction, const FHitResult& HitResult);
	// Reload相关
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void Server_Reload();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Combat")
	void Multicast_Reload();
	//为本地玩家设置reload动画完成后的通知回调，函数类型和FPlayMontageAnimNotifyDelegate 保持一致;
	//该函数必须加上UFUNCTION宏，否则无法绑定到动画的通知点
	UFUNCTION()
	void OnReloadAmmoNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

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
	//当前弹夹容量和当前弹夹中的子弹数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Weapon")
	int MaxAmmo=32;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Weapon")
	int CurrentAmmo=32;

private:
	// 用于存储武器的拥有者
	TObjectPtr<AMyFTCharacter> WeaponOwner;
	float baseDamage = 10.0f;
	float weaponRange = 1000.0f;//1000表示100米
};