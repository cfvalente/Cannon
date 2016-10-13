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
	FVector Speed;
	FTransform Transform;
	bool timetodie;
	float lifetime;

	FVector AShell::CustomAxisRotation(FVector vec, float degrees);
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

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

	

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true")) class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UAudioComponent* FireSound;

	static float DamageZone;
	static float DamageStrength;
	static float PushZone;
	static float PushStrength;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UParticleSystemComponent* ExplosionEffect;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UAudioComponent* ExplosionSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UParticleSystemComponent* SmokeEffect;
	
	
};
