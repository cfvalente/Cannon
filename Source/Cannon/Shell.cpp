// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Shell.h"
#include "Castle.h"
#include "HighTechPowerUp.h"
#include "NukePowerUp.h"
#include "EngineUtils.h" 

float AShell::DamageZone;
float AShell::DamageStrength;
float AShell::PushZone;
float AShell::PushStrength;


// Sets default values
AShell::AShell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Colocar Valor certo

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(20.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Pawn");
	CollisionComponent->OnComponentHit.AddDynamic(this, &AShell::OnHit);		// set up a notification for when this component hits something blocking


	Shell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shell"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShelllObject(TEXT("/Game/bala")); // wherein /Game/ is the Content folder.
	Shell->SetupAttachment(RootComponent);
	Shell->SetStaticMesh(ShelllObject.Object);

	USoundWave *loadedSoundWave;
	FireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire"));
	static ConstructorHelpers::FObjectFinder<USoundWave> Sound(TEXT("/Game/fire"));
	loadedSoundWave = Sound.Object;
	FireSound->SetSound(loadedSoundWave);
	FireSound->bStopWhenOwnerDestroyed = false;
	FireSound->Play(0.0f);

	USoundWave *loadedExplosionSoundWave;
	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Explosion"));
	static ConstructorHelpers::FObjectFinder<USoundWave> ESound(TEXT("/Game/explosion"));
	loadedExplosionSoundWave = ESound.Object;
	ExplosionSound->SetSound(loadedExplosionSoundWave);
	ExplosionSound->bStopWhenOwnerDestroyed = false;
	ExplosionSound->bAutoActivate = false;


	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_Explosion"));
	ExplosionEffect->SetupAttachment(Shell);
	ExplosionEffect->bAutoActivate = false;
	ExplosionEffect->SetRelativeLocation(this->Location);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (P_Explosion.Succeeded())
		ExplosionEffect->SetTemplate(P_Explosion.Object);

	ExplosionProtEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_ExplosionProt"));
	ExplosionProtEffect->SetupAttachment(Shell);
	ExplosionProtEffect->bAutoActivate = false;
	ExplosionProtEffect->SetRelativeLocation(this->Location);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_ExplosionProt(TEXT("/Game/StarterContent/Particles/P_ExplosionProt.P_ExplosionProt"));
	if (P_ExplosionProt.Succeeded())
		ExplosionProtEffect->SetTemplate(P_ExplosionProt.Object);

	SmokeEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke"));
	SmokeEffect->SetupAttachment(Shell);
	SmokeEffect->SetRelativeLocation(this->Location);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Smoke(TEXT("/Game/StarterContent/Particles/P_Steam.P_Steam"));
	if (P_Smoke.Succeeded())
		SmokeEffect->SetTemplate(P_Smoke.Object);


	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	//ProjectileMovement->InitialSpeed = 3000.f;
	//ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after x seconds by default
	InitialLifeSpan = 60.0f;

	// Initial explosion parameters
	PushZone = 800.0f;
	PushStrength = 1200.0f;
	DamageStrength = 38000.0f;
	DamageZone = 1700.0f;
	Nukecycle = 0;
}

// Called when the game starts or when spawned
void AShell::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SmokeEffect->->SetWorldTransform(this->GetTransform());
	//Location = Location + DeltaTime * Speed * 0.1;
	//SetActorLocation(Location);

	if (this->timetodie)
		this->lifetime += DeltaTime;

	if (this->lifetime >= 8.0f)
	{
		Destroy();
		this->timetodie = false;
	}
}

void AShell::Init(FVector Location, float speed, FTransform Transform)
{
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




void AShell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ExplosionSound->Play(0.0f);
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor->IsA(AShell::StaticClass())))
	{
		ExplosionEffect->Activate(true);
		this->timetodie = true;
		SmokeEffect->Deactivate();
		Shell->SetVisibility(false);
		Shell->SetEnableGravity(false);
		ProjectileMovement->Velocity = FVector(0.0f, 0.0f, 0.0f);
		SetActorEnableCollision(false);
		AShell *AS = Cast<AShell>(OtherActor);
		if (AS != NULL)
		{
			AS->SetActorEnableCollision(false);
			AS->Destroy();
		}
	}
	else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherComp->GetName() != "Protection"))
	{
		AShell::RegularExplosion(PushZone, PushStrength, DamageStrength, DamageZone);
		ExplosionEffect->Activate(true);


		this->timetodie = true;
		//Destroy();
		SmokeEffect->Deactivate();
		Shell->SetVisibility(false);
		SetActorEnableCollision(false);
		ProjectileMovement->Velocity = FVector(0.0f, 0.0f, 0.0f);

		
		// Only add impulse and destroy projectile if we hit a physics
		/*if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics() && !(OtherActor->IsA(ACastle::StaticClass())))
		{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
		}*/

		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor->IsA(AHighTechPowerUp::StaticClass())))
		{
			if (OtherComp->IsA<UDestructibleComponent>() && OtherComp->GetName().Equals("Target"))
			{
				//OtherComp->SetEnableGravity(true);
				// LIGAR del do playercannon aqui
				AHighTechPowerUp *HT = Cast<AHighTechPowerUp>(OtherActor);
				HT->Hit();
				for (TObjectIterator<UDestructibleComponent> Itr; Itr; ++Itr)
				{
					// Access the subclass instance with the * or -> operators.
					UDestructibleComponent *Component = *Itr;
					//GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, Itr->GetName());

					if (Component)
					{
						Component->AddRadialImpulse(GetActorLocation(), PushZone, PushStrength, ERadialImpulseFalloff::RIF_Linear, true);
						Component->ApplyRadiusDamage(993402823346297367662189621542912.0f, GetActorLocation(), 100.0f, 2, 1);
					}
				}
				/*
				//for (UActorComponent* Component : OtherActor->GetComponents())
				{
				//GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, Component->GetName());
				}*/

			}

		}

		else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor->IsA(ANukePowerUp::StaticClass())))
		{
			if (OtherComp->IsA<UDestructibleComponent>() && OtherComp->GetName().Equals("Target"))
			{
				ANukePowerUp *Nuk = Cast<ANukePowerUp>(OtherActor);
				Nuk->Hit();
				for (TObjectIterator<UDestructibleComponent> Itr; Itr; ++Itr)
				{
					UDestructibleComponent *Component = *Itr;
					if (Component)
					{
						Component->AddRadialImpulse(GetActorLocation(), PushZone, PushStrength, ERadialImpulseFalloff::RIF_Linear, true);
						Component->ApplyRadiusDamage(993402823346297367662189621542912.0f, GetActorLocation(), 100.0f, 2, 1);
					}
				}
			}
		}
	}
	else
	{
		ExplosionProtEffect->Activate(true);
		this->timetodie = true;
		SmokeEffect->Deactivate();
		Shell->SetVisibility(false);
		SetActorEnableCollision(false);
		ProjectileMovement->Velocity = this->Transform.TransformVector(FVector(0.0f, 0.0f, 0.0f));
	}
}



void AShell::RegularExplosion(float PushZone, float PushStrength, float DamageStrength, float DamageZone)
{
	for (TObjectIterator<UStaticMeshComponent> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		UStaticMeshComponent *Component = *Itr;
		//GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, Itr->GetName());

		if (Component)
		{
			Component->AddRadialImpulse(GetActorLocation(), PushZone, PushStrength, ERadialImpulseFalloff::RIF_Linear, true);
		}
	}

	for (TObjectIterator<UDestructibleComponent> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		UDestructibleComponent *Component = *Itr;
		//GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, Itr->GetName());

		if (Component)
		{
			Component->AddRadialImpulse(GetActorLocation(), PushZone, PushStrength, ERadialImpulseFalloff::RIF_Linear, true);
			Component->ApplyRadiusDamage(DamageStrength, GetActorLocation(), DamageZone, 2, 1);
		}
	}
}