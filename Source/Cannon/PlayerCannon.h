// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerCannon.generated.h"

UCLASS()
class CANNON_API APlayerCannon : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCannon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	FVector CamDir;
	float Ang;

	void MoveZ(float AxisValue);
	void MoveY(float AxisValue);
	void MoveTurret(float AxisValue);
	void Zoom(float AxisValue);

	UCameraComponent* OurCamera;
	UPROPERTY(EditAnywhere) USceneComponent* OurVisibleComponent;





	/** Static Mesh Comp, Set In BP Default Properties */
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UStaticMeshComponent* CannonBarrel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UStaticMeshComponent* CannonBody;
	
};
