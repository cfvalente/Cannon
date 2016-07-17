// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Shell.generated.h"

UCLASS()
class CANNON_API AShell : public AActor
{
	GENERATED_BODY()

private:
	FVector	Location;
	FRotator Angle;
	FVector Acceleration, Speed;
	FTransform Transform;

	FVector AShell::CustomAxisRotation(FVector vec, float degrees);
	
public:	
	// Sets default values for this actor's properties
	AShell();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Init(FVector Location, float speed, FTransform Transform);

	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UStaticMeshComponent* Shell;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class USphereComponent* CollisionComponent;
	
};
