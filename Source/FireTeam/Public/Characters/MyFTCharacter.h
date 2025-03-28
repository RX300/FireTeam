// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireTeamCharacter.h"
#include "MyFTCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChangedDelegate, float, Health);

UCLASS()
class FIRETEAM_API AMyFTCharacter : public AFireTeamCharacter
{
	GENERATED_BODY()
public:
	AMyFTCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//TakeDamage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//获取服务器端人物的旋转量
	UFUNCTION(BlueprintCallable, Category = "Character")
	void FromServerSetCharacterRotation();
	//给动画蓝图设置瞄准偏移量
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetAimOffset();
	//设置服务器端的第三人称骨骼网格体组件
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetThirdPersonMesh(USkeletalMeshComponent* ThirdMesh);
	// 声明一个蓝图可实现的事件
	UFUNCTION(BlueprintImplementableEvent, Category = "CustomEvents")
	void OnCustomEvent();
	//更新人物信息相关
	FHealthChangedDelegate OnHealthChanged;
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Character")
	void Multicast_OnHealthChanged(float DamageCount);
	//人物死亡时的事件
	UFUNCTION(NetMulticast, Reliable,BlueprintCallable, Category = "Character")
	void Multicast_OnDeath();
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Character")
	void Client_OnDeath();
	//RespawnRequest
	UFUNCTION(BlueprintCallable, Category = "Character")
	void RespawnRequest();
	//Refresh HUD
	UFUNCTION(BlueprintCallable, Category = "Character")
	void RefreshHUD();
protected:
	virtual void BeginPlay();
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerComponent) override;
	// End of APawn interface
public:
	//第三人称骨骼网格体组件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	USkeletalMeshComponent* ThirdPersonLegMesh;
	//第三人称骨骼网格体组件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	USkeletalMeshComponent* FirstPersonShadowMesh;
	//服务器端人物的旋转量，用于同步客户端的旋转，在蓝图中可读，编辑器不可见
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character")
	FRotator CharacterControllerRotation;
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float Yaw=0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float Pitch=0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<USkeletalMeshComponent>TP_Mesh;
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	TObjectPtr<UCameraComponent>TPDeath_Camera;
	//人物状态
	//血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Character Info")
	float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Character Info")
	float CurrentHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Character Info")
	bool isDead = false;
private:
	TObjectPtr<UChildActorComponent>TP_Gun;
	TObjectPtr<UChildActorComponent>FP_Gun;
};
