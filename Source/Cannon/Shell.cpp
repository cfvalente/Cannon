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
	Shell->SetWorldRotation(FRotator(Transform.Rotator().Pitch, Transform.Rotator().Yaw, Transform.Rotator().Roll - 180.0f));

	float x, y, z;
	x = FMath::Cos(PI - PI * Transform.Rotator().Euler().X / 180.0f) * FMath::Sin(-PI * Transform.Rotator().Euler().Z / 180.0f);
	y = FMath::Cos(PI - PI * Transform.Rotator().Euler().X / 180.0f) * FMath::Cos(-PI * Transform.Rotator().Euler().Z / 180.0f);
	z = FMath::Sin(PI - PI * Transform.Rotator().Euler().X / 180.0f);

	SpeedVec = FVector(x * speed, y * speed, z * speed);


	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Ang Roll =") + FString::SanitizeFloat(PI - PI * Transform.Rotator().Euler().X / 180.0f));
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Ang Yaw =") + FString::SanitizeFloat(PI * Transform.Rotator().Euler().Z / 180.0f));

	this->Speed = SpeedVec;
	this->Location = Location + FVector(x * 200.0f, y * 200.0f, z * 200.0f);
	this->Transform = Transform;

}


FVector AShell::CustomAxisRotation(FVector vec, float degrees)
{
	float MCos = 1.0f - FMath::Cos(PI - PI * degrees / 180.0f);
	float Cos = FMath::Cos(PI - PI * degrees / 180.0f);
	float Sin = FMath::Sin(PI - PI * degrees / 180.0f);
	float ux, uy, uz;

	FVector NormVec = FVector(vec);
	NormVec.Normalize();

	ux = NormVec.Y;
	uy = NormVec.Z;
	uz = NormVec.X;

	FMatrix CAxis = FMatrix();
	CAxis.M[0][0] = Cos + ux*ux*MCos;
	CAxis.M[0][1] = ux*uy*MCos - uz*Sin;
	CAxis.M[0][2] = ux*uz*MCos + uy*Sin;
	CAxis.M[0][3] = 0.0f;

	CAxis.M[1][0] = uy*ux*MCos + uz*Sin;
	CAxis.M[1][1] = Cos + uy*uy*MCos;
	CAxis.M[1][2] = uy*uz*MCos - ux*Sin;
	CAxis.M[1][3] = 0.0f;

	CAxis.M[2][0] = uz*ux*MCos - uy*Sin;
	CAxis.M[2][1] = uz*uy*MCos + ux*Sin;
	CAxis.M[2][2] = Cos + uz*uz*MCos;
	CAxis.M[2][3] = 0.0f;

	CAxis.M[3][0] = 0.0f;
	CAxis.M[3][1] = 0.0f;
	CAxis.M[3][2] = 0.0f;
	CAxis.M[3][3] = 1.0f;

	return FTransform(CAxis).TransformVector(vec);
}
