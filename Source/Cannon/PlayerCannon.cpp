// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "PlayerCannon.h"


// Sets default values
APlayerCannon::APlayerCannon()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraDirection = FVector(0.0f, 0.0f, 0.0f);

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

	CountingTime = false;

}

// Called when the game starts or when spawned
void APlayerCannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCannon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!CameraDirection.IsZero())
	{
		FVector NewLocation = OurCamera->GetComponentLocation() + CameraDirection * DeltaTime;
		OurCamera->SetWorldLocation(NewLocation);
	}

	{
		FRotator NewAngle = FRotator(CannonBarrel->GetComponentRotation() + FRotator(0.0f, 0.0f, Ang));
		NewAngle.Roll = FMath::Clamp(NewAngle.Roll, 0.0f, 90.0f);
		CannonBarrel->SetRelativeRotation(NewAngle);

	}

	if (CountingTime)
	{
		ChargeTime = ChargeTime + DeltaTime;
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, FString::SanitizeFloat(ChargeTime));
	}

}

// Called to bind functionality to input
void APlayerCannon::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MoveZ", this, &APlayerCannon::MoveZ);
	InputComponent->BindAxis("MoveY", this, &APlayerCannon::MoveY);
	InputComponent->BindAxis("Zoom", this, &APlayerCannon::Zoom);
	InputComponent->BindAxis("BarrelRotation", this, &APlayerCannon::MoveTurret);
	InputComponent->BindAction("CannonFire", IE_Pressed, this, &APlayerCannon::BeginFire);
	InputComponent->BindAction("CannonFire", IE_Released, this, &APlayerCannon::EndFire);
	

}

void APlayerCannon::MoveZ(float AxisValue)
{
	CameraDirection.Z = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
}

void APlayerCannon::MoveY(float AxisValue)
{
	CameraDirection.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
}

void APlayerCannon::Zoom(float AxisValue)
{
	CameraDirection.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 2500.0f;
}

void APlayerCannon::MoveTurret(float AxisValue)
{
	Ang = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1.0f;
}

void APlayerCannon::BeginFire()
{
	CountingTime = true;
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, TEXT("Chargin:"));
}

void APlayerCannon::EndFire()
{
	CountingTime = false;
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Firing!"));
	//Falta spawnar a bala na posicao relativa a Ang e com a forca relativa a ChargeTime
	ChargeTime = 0;
}

