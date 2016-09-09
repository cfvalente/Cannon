// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Castle.h"


// Sets default values
ACastle::ACastle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	Castle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Castle"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CastleObject(TEXT("/Game/castelo")); // wherein /Game/ is the Content folder.
	Castle->SetupAttachment(RootComponent);
	Castle->SetStaticMesh(CastleObject.Object);
	Castle->SetSimulatePhysics(1);

	Castle2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Castle2"));
	Castle2->SetupAttachment(Castle);
	Castle2->SetStaticMesh(CastleObject.Object);
	Castle2->SetSimulatePhysics(1);
	Castle2->SetRelativeLocation(FVector(130.0f, 0.0f, 0.0f));

	Castle3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Castle3"));
	Castle3->SetupAttachment(Castle);
	Castle3->SetStaticMesh(CastleObject.Object);
	Castle3->SetSimulatePhysics(1);
	Castle3->SetRelativeLocation(FVector(-130.0f, 0.0f, 0.0f));

	Castle4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Castle4"));
	Castle4->SetupAttachment(Castle);
	Castle4->SetStaticMesh(CastleObject.Object);
	Castle4->SetSimulatePhysics(1);
	Castle4->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));

}

// Called when the game starts or when spawned
void ACastle::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACastle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
