// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Shell.h"
#include "NukeShell.h"
#include "HTShell.h"
#include "PlayerCannon.h"





// Sets default values
APlayerCannon::APlayerCannon()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	SetActorLocation(FVector::ZeroVector);



	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->SetRelativeLocation(FVector::ZeroVector);

	CannonBarrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonBarrel"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonBarrelObject(TEXT("/Game/cano")); // wherein /Game/ is the Content folder.
	CannonBarrel->SetupAttachment(RootComponent);
	CannonBarrel->SetStaticMesh(CannonBarrelObject.Object);
	CannonBarrel->SetRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));

	CannonBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonBody"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonBodyObject(TEXT("/Game/canhao")); // wherein /Game/ is the Content folder.
	CannonBody->SetupAttachment(RootComponent);
	CannonBody->SetStaticMesh(CannonBodyObject.Object);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CannonBarrel);
	SpringArm->SetRelativeLocation(FVector(0.0f, -250.0f, -70.0f));
	SpringArm->SetRelativeRotation(FRotator(165.0f, 90.0f, 0.0f));
	
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurCamera->SetupAttachment(SpringArm);
	OurCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	OurCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	OurCamera->Activate();

	FreeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FreeCamera"));
	FreeCamera->Deactivate();

	Mode = CameraMode::muzzle;
	CountingTime = false;
	CameraDirection = FVector(0.0f, 0.0f, 0.0f);



}

// Called when the game starts or when spawned
void APlayerCannon::BeginPlay()
{
	NewTransform = FTransform(CannonBarrel->GetComponentRotation());

	InitialAngleRoll = CannonBarrel->GetComponentRotation().Roll;
	InitialAngleYaw = CannonBarrel->GetComponentRotation().Yaw;
	displayAng = 0.0f;
	HTShell = NukeShell = false;

	AShell::DamageZone = 10.0f;
	AShell::DamageStrength = 100.0f;
	AShell::PushZone = 10.0f;
	AShell::PushStrength = 10.0f;

	del.AddDynamic(this, &APlayerCannon::HTPowerUpHit);
	Super::BeginPlay();
}

// Called every frame
void APlayerCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		if (!CameraDirection.IsZero())
		{
			if (Mode == CameraMode::muzzle)
			{
				FVector NewLocation = SpringArm->GetComponentLocation() + CameraDirection * DeltaTime;
				SpringArm->SetWorldLocation(NewLocation);
				CameraDirection = FVector::ZeroVector;
			}
			else if (Mode == CameraMode::free)
			{
				FVector NewLocation = FreeCamera->GetComponentLocation() + CameraDirection * DeltaTime;
				FreeCamera->SetWorldLocation(NewLocation);
				CameraDirection = FVector::ZeroVector;
			}
		}


	if (!FMath::IsNearlyZero(AngRoll) || !FMath::IsNearlyZero(AngYaw))
	{
		if (Mode == CameraMode::muzzle)
		{
			float NewRoll, NewYaw;
			//FRotator Rot = CannonBarrel->GetRelativeTransform().Rotator();								// Rotacao relativa ao objeto base
			FRotator Rot = CannonBarrel->GetComponentRotation();											// Rotacao em relacao às coordenadas do mundo - nao em relacao ao objeto pai!
			NewRoll = FMath::ClampAngle(AngRoll + Rot.Roll, InitialAngleRoll - 90.0f, InitialAngleRoll + 10.0f);
			NewYaw = Rot.Yaw - AngYaw;
			NewTransform = FTransform(FRotator(Rot.Pitch, NewYaw, NewRoll));
			CannonBarrel->SetWorldRotation(NewTransform.Rotator());

			displayAng = 180.0f - NewRoll;
			if (displayAng > 180.0f) displayAng = displayAng - 360.0f;
		}
		else if (Mode == CameraMode::free)
		{
			float NewPitch, NewYaw;
			FRotator Rot = FreeCamera->GetComponentRotation();
			if (Rot.Pitch >= 89.0f && AngRoll < 0.0f) AngRoll = 0.0f;
			else if (Rot.Pitch <= -89.0f && AngRoll > 0.0f) AngRoll = 0.0f;
			NewPitch = Rot.Pitch - 1.5f * AngRoll;
			NewYaw = Rot.Yaw - 1.5f * AngYaw;
			FreeCamera->SetWorldRotation(FTransform(FRotator(NewPitch, NewYaw, 0.0f)).Rotator());
		}

	}

	if (CountingTime)
		ChargeTime = ChargeTime + DeltaTime;


}

// Called to bind functionality to input
void APlayerCannon::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MoveZ", this, &APlayerCannon::MoveZ);
	InputComponent->BindAxis("MoveY", this, &APlayerCannon::MoveY);
	InputComponent->BindAxis("Zoom", this, &APlayerCannon::Zoom);
	InputComponent->BindAxis("BarrelRotationRoll", this, &APlayerCannon::MoveTurretRoll);
	InputComponent->BindAxis("BarrelRotationYaw", this, &APlayerCannon::MoveTurretYaw);
	InputComponent->BindAction("CannonFire", IE_Pressed, this, &APlayerCannon::BeginFire);
	InputComponent->BindAction("CannonFire", IE_Released, this, &APlayerCannon::EndFire);
	InputComponent->BindAction("ToggleCamera", IE_Pressed, this, &APlayerCannon::ToggleCamera);


}


void APlayerCannon::ToggleCamera()
{
	if (Mode == CameraMode::free)
	{
		Mode = CameraMode::muzzle;
		SpringArm->SetRelativeLocation(FVector(0.0f, -250.0f, -70.0f));
		SpringArm->SetRelativeRotation(FRotator(165.0f, 90.0f, 0.0f));
		FreeCamera->Deactivate();
		OurCamera->Activate();
		GEngine->AddOnScreenDebugMessage(-1, 0.14f, FColor::Yellow, "Muzzle");
	}
	else if (Mode == CameraMode::muzzle)
	{
		Mode = CameraMode::free;
		OurCamera->Deactivate();
		FreeCamera->Activate();
		FreeCamera->SetWorldTransform(OurCamera->GetComponentTransform());
		GEngine->AddOnScreenDebugMessage(-1, 0.14f, FColor::Yellow, "Free");
	}
}

void APlayerCannon::MoveZ(float AxisValue)
{
	if (Mode == CameraMode::muzzle)
	{
		CameraDirection += CannonBarrel->GetUpVector() * FMath::Clamp(AxisValue, -1.0f, 1.0f) * -1000.0f;
	}
	else if (Mode == CameraMode::free)
	{
		CameraDirection += FreeCamera->GetUpVector() * FMath::Clamp(AxisValue, -1.0f, 1.0f) * 3000.0f;
	}

}

void APlayerCannon::MoveY(float AxisValue)
{
	if(Mode == CameraMode::muzzle)
	{
		CameraDirection += CannonBarrel->GetForwardVector() * FMath::Clamp(AxisValue, -1.0f, 1.0f) * -1000.0f;
	}
	else if (Mode == CameraMode::free)
	{
		CameraDirection += FreeCamera->GetRightVector() * FMath::Clamp(AxisValue, -1.0f, 1.0f) * 3000.0f;
	}
}

void APlayerCannon::Zoom(float AxisValue)
{
	if (Mode == CameraMode::muzzle)
	{
		CameraDirection += CannonBarrel->GetRightVector() * FMath::Clamp(AxisValue, -1.0f, 1.0f) * -1000.0f;
	}
	else if (Mode == CameraMode::free)
	{
		CameraDirection += FreeCamera->GetForwardVector() * FMath::Clamp(AxisValue, -1.0f, 1.0f) * 3000.0f;
	}
}

void APlayerCannon::MoveTurretRoll(float AxisValue)
{
		AngRoll = -FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1.0f;
}

void APlayerCannon::MoveTurretYaw(float AxisValue)
{
		AngYaw = -FMath::Clamp(AxisValue, -1.0f, 1.0f) * 2.0f;
}

void APlayerCannon::BeginFire()
{
	CountingTime = true;
}

void APlayerCannon::EndFire()
{
	float Speed;
	CountingTime = false;


	ChargeTime += 1.0f;
	Speed = FMath::Clamp(ChargeTime, 1.0f, 2.5f) * 25000.0f;

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		AShell* Shell;
		// spawn the projectile at the muzzle
		if (NukeShell)
		{
			Shell = World->SpawnActor<ANukeShell>(ANukeShell::StaticClass());
			HTShell = false;
			NukeShell = false;
		}
		else if (HTShell)
		{
			Shell = World->SpawnActor<AHTShell>(AHTShell::StaticClass());
		}
		else Shell = World->SpawnActor<AShell>(AShell::StaticClass());
		Shell->Init(this->GetActorLocation(), Speed, NewTransform);
		
	}

	ChargeTime = 0.0f;
}


void APlayerCannon::HTPowerUpHit()
{
	//PowerUp += 10000000.0f;
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Yellow, "OK!!!!!!");
	HTShell = true;
	NukeShell = true;
}