// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Pawn.h"
#include "PlayerCannon.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHTPowerUpDelagate);



UCLASS()
class CANNON_API APlayerCannon : public APawn
{
	GENERATED_BODY()

private:
	float AngRoll,AngYaw, ChargeTime;
	FVector CameraDirection;
	FTransform NewTransform;
	bool CountingTime;
	float InitialAngleRoll;
	float InitialAngleYaw;
	bool HTShell;
	bool NukeShell;

	enum class CameraMode { muzzle, free };
	CameraMode Mode;
//	int CameraMode;



public:
	// Sets default values for this pawn's properties
	APlayerCannon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	FHTPowerUpDelagate del;


	void MoveZ(float AxisValue);
	void MoveY(float AxisValue);
	void MoveTurretRoll(float AxisValue);
	void MoveTurretYaw(float AxisValue);
	void Zoom(float AxisValue);
	void BeginFire();
	void EndFire();
	void ToggleCamera();

	UFUNCTION() void HTPowerUpHit();

	//UCameraComponent* OurCamera;
	UPROPERTY(EditAnywhere) UCameraComponent* OurCamera;
	UPROPERTY(EditAnywhere) UCameraComponent* FreeCamera;
	UPROPERTY(EditAnywhere) USpringArmComponent* SpringArm;
	//UPROPERTY(EditAnywhere) USceneComponent* OurVisibleComponent;





	/** Static Mesh Comp, Set In BP Default Properties */
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UStaticMeshComponent* CannonBarrel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UStaticMeshComponent* CannonBody;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float displayAng;


};

