// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Castle.generated.h"

UCLASS()
class CANNON_API ACastle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACastle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UStaticMeshComponent* Castle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UStaticMeshComponent* Castle2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UStaticMeshComponent* Castle3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UStaticMeshComponent* Castle4;



};
