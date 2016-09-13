// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DestructibleMesh.h"
#include "GameFramework/Actor.h"
#include "Castle.generated.h"

UCLASS()
class CANNON_API ACastle : public ADestructibleActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACastle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UDestructibleComponent* Castle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UDestructibleComponent* Castle2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UDestructibleComponent* Castle3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UDestructibleComponent* Castle4;



};
