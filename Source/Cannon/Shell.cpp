// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Shell.h"


// Sets default values
AShell::AShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Colocar Valor certo
	

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//RootComponent->SetRelativeLocation(FVector::ZeroVector);

	Shell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shell"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonBarrelObject(TEXT("/Game/bala")); // wherein /Game/ is the Content folder.
	//Shell->SetupAttachment(RootComponent);
	this->AttachRootComponentTo(Shell);
	Shell->SetStaticMesh(CannonBarrelObject.Object);

	
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

	Location = Location + DeltaTime * Speed /*- 0.5 * Acceleration * DeltaTime * DeltaTime*/;
	//SetActorLocation(Location);
	Shell->SetWorldLocation(Location);



}

void AShell::Init(FVector Location, FRotator Angle, float Speed)
{
	Acceleration = Angle.RotateVector(FVector(0.0f, -0.0000010f, 0.0f));
	//SetActorLocation(Location);
	//SetActorRotation(Angle);
	Shell->SetWorldLocation(Location);
	Shell->SetWorldRotation(Angle);
	this->Speed = Angle.RotateVector(FVector(0.0f, Speed, 0.0f));
	this->Location = Location;
	this->Angle = Angle;
	//this->SetActorEnableCollision(true);
	//this->SetLifeSpan(30);
	Shell->SetSimulatePhysics(true);
	Shell->WakeRigidBody();
	//Shell->SetEnableGravity(true);
	//Shell->SetMassOverrideInKg("XX", 30.0f, true);
}
