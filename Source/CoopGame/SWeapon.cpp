// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "CoopGame.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef DebugWeapon(TEXT("COOP.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug lines for weapons"), ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

void ASWeapon::Fire()
{
	AActor* myOwner = GetOwner();
	if(myOwner)
	{
		FVector eyeLoc;
		FRotator eyeRot;
		myOwner->GetActorEyesViewPoint(eyeLoc, eyeRot);

		FVector shootDirection = eyeRot.Vector();
		FVector traceEnd = eyeLoc + (shootDirection * 10000.0f);
		FVector tracerEnd{traceEnd};
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		
		FCollisionQueryParams params;
		params.bTraceComplex = true;
		params.bReturnPhysicalMaterial = true;
		
		
		FHitResult hit;
		bool wasHit = GetWorld()->LineTraceSingleByChannel(hit, MuzzleLocation, traceEnd, ECollisionChannel::COLLISION_WEAPON, params);
		if(wasHit)
		{
			AActor* hitActor = hit.GetActor();
			if(IsValid(hitActor))
			{
				UGameplayStatics::ApplyPointDamage(hitActor, 20.0f, shootDirection, hit, myOwner->GetInstigatorController(), this, DamageType);
			}
			
			tracerEnd = hit.ImpactPoint;
			
			UParticleSystem* selectedParticle;
			EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hit.PhysMaterial.Get());

			switch(surfaceType)
			{
			case SURFACE_FLESHDEFAULT:
			case SURFACE_FLESHVULNERABLE:
				selectedParticle = FlashImpactEffect;
				break;
			default:
				selectedParticle = DefaultImpactEffect;
				break;
			}

			if(selectedParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectedParticle, hit.ImpactPoint, hit.ImpactNormal.Rotation());
			}

		}

		if(DebugWeaponDrawing > 0)
			DrawDebugLine(GetWorld(), MuzzleLocation, traceEnd, FColor::White, false, 1.0f, 0, 1.0f);

		PlayFireEffects(MuzzleLocation, tracerEnd);
	}
}

void ASWeapon::PlayFireEffects(FVector& MuzzleLocation, FVector& TraceEnd)
{
	if(MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if(TracerEffect)
	{
		UParticleSystemComponent* tracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if(tracerComponent)
		{
			tracerComponent->SetVectorParameter("BeamEnd", TraceEnd);
		}
	}

	APawn* owner = Cast<APawn>(GetOwner());
	if(owner)
	{
		APlayerController* pc = Cast<APlayerController>(owner->GetController());
		if(pc)
		{
			pc->ClientStartCameraShake(FireShake);
		}
	}
	
}


