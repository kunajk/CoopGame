// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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
		
		FHitResult hit;
		bool wasHit = GetWorld()->LineTraceSingleByChannel(hit, MuzzleLocation, traceEnd, ECollisionChannel::ECC_Visibility, params);
		if(wasHit)
		{
			AActor* hitActor = hit.GetActor();
			if(IsValid(hitActor))
			{
				UGameplayStatics::ApplyPointDamage(hitActor, 20.0f, shootDirection, hit, myOwner->GetInstigatorController(), this, DamageType);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitActorEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation());
				tracerEnd = hit.ImpactPoint;
			}
			else
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation());
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
}


