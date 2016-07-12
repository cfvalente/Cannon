// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "PlayerCannon.h"


// Sets default values
APlayerCannon::APlayerCannon()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));

	//OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetWorldLocation(FVector(-450.0f, 0.0f, 450.0f));
	OurCamera->SetWorldRotation(FRotator(-15.0f, 0.0f, 0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);


	CannonBarrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonBarrel"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonBarrelObject(TEXT("/Game/cano")); // wherein /Game/ is the Content folder.
	CannonBarrel->SetStaticMesh(CannonBarrelObject.Object);

	CannonBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonBody"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonBodyObject(TEXT("/Game/canhao")); // wherein /Game/ is the Content folder.
	CannonBody->SetStaticMesh(CannonBodyObject.Object);
	// Qualquer coisa em qualquer lugar.

}

// Called when the game starts or when spawned
void APlayerCannon::BeginPlay()
{
	CamDir = FVector(0.0f, 0.0f, 0.0f);
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCannon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!CamDir.IsZero())
	{
		FVector NewLocation = OurCamera->GetComponentLocation() + CamDir * DeltaTime;
		OurCamera->SetWorldLocation(NewLocation);
	}

	{
		CannonBarrel->SetRelativeRotation(CannonBarrel->GetComponentRotation() + FRotator(0.0f, 0.0f, Ang));

	}


	/*{
		FVector NewLocation = CannonBarrel->GetComponentLocation() + FVector(0,0,250) * DeltaTime;
		CannonBarrel->SetWorldLocation(NewLocation);

	}*/

}

// Called to bind functionality to input
void APlayerCannon::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MoveZ", this, &APlayerCannon::MoveZ);
	InputComponent->BindAxis("MoveY", this, &APlayerCannon::MoveY);
	InputComponent->BindAxis("BarrelRotation", this, &APlayerCannon::MoveTurret);

}

void APlayerCannon::MoveZ(float AxisValue)
{
	CamDir.Z = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
}

void APlayerCannon::MoveY(float AxisValue)
{
	CamDir.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
}

void APlayerCannon::MoveTurret(float AxisValue)
{
	Ang = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 10.0f;
	//CamDir.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;


}

