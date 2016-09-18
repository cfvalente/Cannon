// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Shell.h"
#include "Castle.h"
#include "EngineUtils.h" 


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
	

	/*OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(Shell);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}*/


	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	//ProjectileMovement->InitialSpeed = 3000.f;
	//ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 6.0f;
	DamageZone = 10.0f;
	DamageStrength = 100.0f;
	PushZone = 10.0f;
	PushStrength = 10.0f;
	/*AShell::DamageZone = 10.0f;
	AShell::DamageStrength = 100.0f;
	AShell::PushZone = 10.0f;
	AShell::PushStrength = 10.0f;*/
}

// Called when the game starts or when spawned
void AShell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShell::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Location = Location + DeltaTime * Speed * 0.1;
	//SetActorLocation(Location);



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

	this->Location = Location + this->Transform.TransformVector(FVector(0.0f, 250.0f, 0.0f));
	SetActorLocation(this->Location);

}




void AShell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
	DamageZone = DamageZone + 10.0f;
	DamageStrength = DamageStrength + 100.0f;
	PushZone = PushZone + 10.0f;
	PushStrength = PushStrength + 10.0f;
	Destroy();

	// Only add impulse and destroy projectile if we hit a physics
	/*if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics() && !(OtherActor->IsA(ACastle::StaticClass())))
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}*/
}
