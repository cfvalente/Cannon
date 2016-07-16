// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/ProjectileMovementComponent.h"
#include "ShellMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CANNON_API UShellMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	
	
	
};
