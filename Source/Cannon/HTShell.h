// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shell.h"
#include "HTShell.generated.h"

/**
 * 
 */
UCLASS()
class CANNON_API AHTShell : public AShell
{
	GENERATED_BODY()


private:


public:
	AHTShell();
	
	virtual void Init(FVector Location, float speed, FTransform Transform);
	
};
