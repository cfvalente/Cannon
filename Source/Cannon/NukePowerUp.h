// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DestructibleMesh.h"
#include "GameFramework/Actor.h"
#include "PlayerCannon.h"
#include "NukePowerUp.generated.h"

UCLASS()
class CANNON_API ANukePowerUp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANukePowerUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UDestructibleComponent* Target;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UDestructibleComponent* Protection;

	virtual void Hit();

	FNukePowerUpDelagate nuke_del;
};
