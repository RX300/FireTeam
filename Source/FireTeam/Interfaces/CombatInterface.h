// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRETEAM_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//ue5里添加接口，如果不在.cpp里做默认实现必须为纯虚函数

	//主要攻击
	//isFiring:是否正在攻击
	virtual void PrimaryFire(bool isFiring)=0;

	//换弹
	virtual void Reload()=0;
};
