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

void AShell::Init(FVector Location, FVector Speed, FTransform Transform)
{
	Shell->SetWorldRotation((Transform * FTransform(FRotator(0.0f, 0.0f, 180.0f))).Rotator());

	//FTransform transf = FTransform(FRotator(0, Transform.Rotator().Euler().Y, Transform.Rotator().Euler().X));
	FVector ts = FVector(0.0f, -FMath::Cos(PI * Transform.Rotator().Euler().X / 180.0f) * Speed.Y, FMath::Abs(FMath::Sin(PI * Transform.Rotator().Euler().X / 180.0f) * Speed.Y));

	/*
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Transform.X =") + FString::SanitizeFloat(Transform.Rotator().Euler().X));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Speed =") + FString::SanitizeFloat(Speed.Y));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("TransfSpeedY =") + FString::SanitizeFloat(transf.TransformVector(Speed).Y));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("TransfSpeedZ =") + FString::SanitizeFloat(transf.TransformVector(Speed).Z));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Sin X =") + FString::SanitizeFloat(FMath::Sin(PI * transf.Rotator().Euler().X / 180.0f)));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Cos X =") + FString::SanitizeFloat(FMath::Cos(PI *  transf.Rotator().Euler().X / 180.0f)));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Sin Y =") + FString::SanitizeFloat(FMath::Sin(PI * transf.Rotator().Euler().Y / 180.0f)));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Cos Y =") + FString::SanitizeFloat(FMath::Cos(PI * transf.Rotator().Euler().Y / 180.0f)));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Test SpeedY =") + FString::SanitizeFloat(ts.Y));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Test SpeedZ =") + FString::SanitizeFloat(ts.Z));
	//this->Speed = transf.TransformVector(Speed);
	*/
	this->Speed = ts;
	this->Location = Location + Transform.TransformVector(FVector(0.0f, -200.0f, 0.0f));
	this->Transform = Transform;

}
