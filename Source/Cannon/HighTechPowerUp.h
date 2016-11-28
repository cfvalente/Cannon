// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DestructibleMesh.h"
#include "GameFramework/Actor.h"
#include "PlayerCannon.h"
#include "HighTechPowerUp.generated.h"

UCLASS()
class CANNON_API AHighTechPowerUp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHighTechPowerUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UDestructibleComponent* Target;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UDestructibleComponent* Protection;

	virtual void Hit();

	FHTPowerUpDelagate del;
};
