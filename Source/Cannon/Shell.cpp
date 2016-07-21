// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Shell.h"


// Sets default values
AShell::AShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Colocar Valor certo
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(40.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));


	Shell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shell"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShelllObject(TEXT("/Game/bala")); // wherein /Game/ is the Content folder.
	Shell->SetupAttachment(RootComponent);
	Shell->SetStaticMesh(ShelllObject.Object);

	/*OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(Shell);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}*/



	
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

	Location = Location + DeltaTime * Speed * 0.1;
	SetActorLocation(Location);



}

void AShell::Init(FVector Location, float speed, FTransform Transform)
{
	FVector SpeedVec;
	this->Transform = FTransform(FRotator(Transform.Rotator().Pitch, Transform.Rotator().Yaw, Transform.Rotator().Roll - 180.0f));

	SetActorTransform(this->Transform);

	this->Speed = this->Transform.TransformVector(FVector(0.0f, speed, 0.0f));
	this->Location = Location + this->Transform.TransformVector(FVector(0.0f, 200.0f, 0.0f));

}
