// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shell.h"
#include "NukeShell.generated.h"

/**
 * 
 */
UCLASS()
class CANNON_API ANukeShell : public AShell
{
	GENERATED_BODY()

protected:
	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		virtual void RegularExplosion(float PushZone, float PushStrength, float DamageStrength, float DamageZone);

	FTimerHandle NukeDelay;

	UFUNCTION()
		void NukeTimerEnd();

public:
	// Sets default values for this actor's properties
	ANukeShell();

	UPROPERTY(BlueprintReadWrite, EditAnywhere) class UParticleSystemComponent* NukeExplosionEffect;
	
	
};
