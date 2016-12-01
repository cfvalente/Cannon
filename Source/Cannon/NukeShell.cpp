// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Shell.h"
#include "Castle.h"
#include "HighTechPowerUp.h"
#include "EngineUtils.h" 
#include "NukeShell.h"



ANukeShell::ANukeShell() : AShell::AShell()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShelllObject(TEXT("/Game/balaN")); // wherein /Game/ is the Content folder.
	Shell->SetStaticMesh(ShelllObject.Object);

	NukeExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_Nuke"));
	NukeExplosionEffect->SetupAttachment(Shell);
	NukeExplosionEffect->bAutoActivate = false;
	NukeExplosionEffect->SetRelativeLocation(this->Location);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(TEXT("/Game/StarterContent/Particles/P_Nuke.P_Nuke"));
	if (P_Explosion.Succeeded())
		NukeExplosionEffect->SetTemplate(P_Explosion.Object);
	NukeExplosionEffect->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
}



void ANukeShell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ExplosionSound->Play(0.0f);
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor->IsA(AShell::StaticClass())))
	{
		NukeExplosionEffect->Activate(true);
		this->timetodie = true;
		SmokeEffect->Deactivate();
		Shell->SetVisibility(false);
		Shell->SetEnableGravity(false);
		ProjectileMovement->Velocity = FVector(0.0f, 0.0f, 0.0f);
		SetActorEnableCollision(false);
		AShell *AS = Cast<AShell>(OtherActor);
		if (AS != NULL)
		{
			AS->SetActorEnableCollision(false);
			AS->Destroy();
		}
	}
	else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherComp->GetName() != "Protection"))
	{
		ANukeShell::RegularExplosion(PushZone, PushStrength, DamageStrength, DamageZone);
		GetWorld()->GetTimerManager().SetTimer(NukeDelay, this, &ANukeShell::NukeTimerEnd, 0.5f, false); //triggers the NUKE after a delay
		NukeExplosionEffect->Activate(true);


		this->timetodie = true;
		SmokeEffect->Deactivate();
		Shell->SetVisibility(false);
		SetActorEnableCollision(false);
		ProjectileMovement->Velocity = FVector(0.0f, 0.0f, 0.0f);


		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor->IsA(AHighTechPowerUp::StaticClass())))
		{
			if (OtherComp->IsA<UDestructibleComponent>() && OtherComp->GetName().Equals("Target"))
			{
				//OtherComp->SetEnableGravity(true);
				// LIGAR del do playercannon aqui
				AHighTechPowerUp *HT = Cast<AHighTechPowerUp>(OtherActor);
				HT->Hit();
				for (TObjectIterator<UDestructibleComponent> Itr; Itr; ++Itr)
				{
					// Access the subclass instance with the * or -> operators.
					UDestructibleComponent *Component = *Itr;
					//GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, Itr->GetName());

					if (Component)
					{
						Component->AddRadialImpulse(GetActorLocation(), PushZone, PushStrength, ERadialImpulseFalloff::RIF_Linear, true);
						Component->ApplyRadiusDamage(993402823346297367662189621542912.0f, GetActorLocation(), 100.0f, 2, 1);
					}
				}
			}

		}
	}
	else
	{
		ExplosionProtEffect->Activate(true);
		this->timetodie = true;
		SmokeEffect->Deactivate();
		Shell->SetVisibility(false);
		SetActorEnableCollision(false);
		ProjectileMovement->Velocity = this->Transform.TransformVector(FVector(0.0f, 0.0f, 0.0f));
	}
}



void ANukeShell::RegularExplosion(float PushZone, float PushStrength, float DamageStrength, float DamageZone)
{
	for (TObjectIterator<UStaticMeshComponent> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		UStaticMeshComponent *Component = *Itr;
		//GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Blue, Itr->GetName());

		if (Component)
		{
			Component->AddRadialImpulse(GetActorLocation(), PushZone, PushStrength, ERadialImpulseFalloff::RIF_Linear, true);
		}
	}

	for (TObjectIterator<UDestructibleComponent> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		UDestructibleComponent *Component = *Itr;

		if (Component)
		{
			Component->AddRadialImpulse(GetActorLocation(), PushZone, PushStrength, ERadialImpulseFalloff::RIF_Linear, true);
			Component->ApplyRadiusDamage(DamageStrength, GetActorLocation(), DamageZone, 2, 1);
		}
	}
}

void ANukeShell::NukeTimerEnd()
{
	Nukecycle = Nukecycle + 1;
	if (Nukecycle<40)
	{
		ANukeShell::RegularExplosion(PushZone + 80.0f*Nukecycle, PushStrength + 80.0f*Nukecycle, DamageStrength + 80.0f*Nukecycle, DamageZone + 80.0f*Nukecycle);
		GetWorld()->GetTimerManager().SetTimer(NukeDelay, this, &ANukeShell::NukeTimerEnd, 0.1f / float(Nukecycle), false);
	}


}