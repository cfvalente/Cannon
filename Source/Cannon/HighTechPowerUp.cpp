// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "HighTechPowerUp.h"


// Sets default values
AHighTechPowerUp::AHighTechPowerUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	Target = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Target"));
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> TargetObjectD(TEXT("/Game/Core_PowerUp_DM")); // wherein /Game/ is the Content folder.
	Target->SetupAttachment(RootComponent);
	Target->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Target->SetDestructibleMesh(TargetObjectD.Object);
	Target->SetEnableGravity(false);
	Target->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));

	Protection = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Protection"));
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> ProtectionObjectD(TEXT("/Game/Periferal_PowerUp_DM")); // wherein /Game/ is the Content folder.
	Protection->SetupAttachment(RootComponent);
	Protection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Protection->SetDestructibleMesh(ProtectionObjectD.Object);
	

}

// Called when the game starts or when spawned
void AHighTechPowerUp::BeginPlay()
{
	for (TObjectIterator<APawn> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		APawn *Pawn = *Itr;
		if (Pawn->IsA(APlayerCannon::StaticClass()))
		{
			class APlayerCannon *PlayerCannon = Cast<APlayerCannon>(Pawn);
			this->del = PlayerCannon->del;
		}
	}
	Super::BeginPlay();
	Target->SetEnableGravity(false);
}

// Called every frame
void AHighTechPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHighTechPowerUp::Hit()
{
	this->del.Broadcast();
}
