// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Castle.h"


// Sets default values
ACastle::ACastle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	Castle = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Castle"));
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> CastleObjectD(TEXT("/Game/castelo_DM")); // wherein /Game/ is the Content folder.
	Castle->SetupAttachment(RootComponent);
	Castle->SetSimulatePhysics(1);
	Castle->SetEnableGravity(1);
	Castle->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Castle->SetDestructibleMesh(CastleObjectD.Object);



	Castle2 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Castle2"));
	Castle2->SetupAttachment(RootComponent);
	Castle2->SetDestructibleMesh(CastleObjectD.Object);
	Castle2->SetSimulatePhysics(1);
	Castle2->SetEnableGravity(1);
	Castle2->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Castle2->SetRelativeLocation(FVector(130.0f, 0.0f, 0.0f));

	Castle3 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Castle3"));
	Castle3->SetupAttachment(RootComponent);
	Castle3->SetDestructibleMesh(CastleObjectD.Object);
	Castle3->SetSimulatePhysics(1);
	Castle3->SetEnableGravity(1);
	Castle3->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Castle3->SetRelativeLocation(FVector(-130.0f, 0.0f, 0.0f));

	Castle4 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Castle4"));
	Castle4->SetupAttachment(RootComponent);
	Castle4->SetDestructibleMesh(CastleObjectD.Object);
	Castle4->SetSimulatePhysics(1);
	Castle4->SetEnableGravity(1);
	Castle4->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
