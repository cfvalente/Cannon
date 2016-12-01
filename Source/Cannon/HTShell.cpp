// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "HTShell.h"





AHTShell::AHTShell() : AShell::AShell()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShelllObject(TEXT("/Game/balaHT")); // wherein /Game/ is the Content folder.
	Shell->SetStaticMesh(ShelllObject.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(TEXT("/Game/StarterContent/Particles/P_HighTech.P_HighTech"));
	if (P_Explosion.Succeeded())
		ExplosionEffect->SetTemplate(P_Explosion.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Smoke(TEXT("/Game/StarterContent/Particles/P_Steam_HT.P_Steam_HT"));
	if (P_Smoke.Succeeded())
		SmokeEffect->SetTemplate(P_Smoke.Object);
	
	ExplosionEffect->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
	
	PushZone = 800.0f * 3.0f;
	PushStrength = 1200.0f * 3.0f;
	DamageStrength = 38000.0f * 1.75f;
	DamageZone = 1700.0f * 1.75f;
}



void AHTShell::Init(FVector Location, float speed, FTransform Transform)
{
	speed = speed*3.0f;
	FVector SpeedVec;
	this->Transform = FTransform(FRotator(Transform.Rotator().Pitch, Transform.Rotator().Yaw, Transform.Rotator().Roll - 180.0f));
	Shell->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	//this->Transform = FTransform(FRotator(0.0f, 45.0f, 0.0f));

	SetActorTransform(this->Transform);

	this->Speed = this->Transform.TransformVector(FVector(0.0f, speed * 0.05f, 0.0f));

	ProjectileMovement->Velocity = this->Transform.TransformVector(FVector(0.0f, speed * 0.05f, 0.0f));

	this->Location = Location + this->Transform.TransformVector(FVector(0.0f, 333.0f, 0.0f));
	SetActorLocation(this->Location);

	ExplosionEffect->SetRelativeScale3D(FVector(PushZone / 230.0f, PushZone / 230.0f, PushZone / 230.0f));
}