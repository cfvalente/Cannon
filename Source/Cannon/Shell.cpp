// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Shell.h"
#include "Castle.h"
#include "EngineUtils.h" 


// Sets default values
AShell::AShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Colocar Valor certo
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(20.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Pawn");
	CollisionComponent->OnComponentHit.AddDynamic(this, &AShell::OnHit);		// set up a notification for when this component hits something blocking


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


	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	//ProjectileMovement->InitialSpeed = 3000.f;
	//ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 6.0f;



	
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

	//Location = Location + DeltaTime * Speed * 0.1;
	//SetActorLocation(Location);



}

void AShell::Init(FVector Location, float speed, FTransform Transform)
{
	FVector SpeedVec;
	this->Transform = FTransform(FRotator(Transform.Rotator().Pitch, Transform.Rotator().Yaw, Transform.Rotator().Roll - 180.0f));
	Shell->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	//this->Transform = FTransform(FRotator(0.0f, 45.0f, 0.0f));


	SetActorTransform(this->Transform);

	this->Speed = this->Transform.TransformVector(FVector(0.0f, speed * 0.05f, 0.0f));

	ProjectileMovement->Velocity = this->Transform.TransformVector(FVector(0.0f, speed * 0.05f, 0.0f));

	this->Location = Location + this->Transform.TransformVector(FVector(0.0f, 250.0f, 0.0f));
	SetActorLocation(this->Location);

}




void AShell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	for (TObjectIterator<UStaticMeshComponent> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		UStaticMeshComponent *Component = *Itr;
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, Itr->GetName());

		if (Component)
		{
			Component->AddRadialImpulse(GetActorLocation(), radius, strength, ERadialImpulseFalloff::RIF_Linear, true);
		}
	}

	/*for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		AActor *Mesh = *ActorItr;
		TArray<USceneComponent> t;
		//ClientMessage(ActorItr->GetName());
		//ClientMessage(ActorItr->GetActorLocation().ToString());
		UStaticMeshComponent* SM = Cast<UStaticMeshComponent>((*ActorItr)->GetRootComponent());
		t = SM->GetAttachChildren();
		if ()
		{
			SM->AddRadialImpulse(GetActorLocation(), radius, strength, ERadialImpulseFalloff::RIF_Linear, true);
		}
	}*/

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics() && !(OtherActor->IsA(ACastle::StaticClass())))
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		//OtherComp->AddRadialImpulse(GetActorLocation(), radius, strength, RIF_Constant, false);


		Destroy();
	}

	/*if (OtherActor->IsA(ACastle::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, FString("LALALALLAAL"));
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		for (auto It = NearbyActors.CreateIterator(); It; It++)
		{
			//Get the static mesh component for each actor
			UStaticMeshComponent* SM = Cast<UStaticMeshComponent>((*It)->GetRootComponent());

			/*If the component is valid, apply a radial impulse from the location of the "Bomb" Actor
			having as Radius and Strength the values from the Editor.
			The RIF_Linear impulse fall off parameter means that we want the impulse to fall off to zero when
			the max distance is reached.
			The last parameter means that we want to ignore the mass of each Static Mesh*/
			/*GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, FString("LALALALLAAL2"));
			if (SM)
			{
				SM->AddRadialImpulse(GetActorLocation(), radius, strength, ERadialImpulseFalloff::RIF_Linear, true);
			}
		}

		//OtherComp->AddRadialImpulse(GetActorLocation(), radius, strength, RIF_Constant, false);
		//Trace
		//Destroy();
	}*/
}
