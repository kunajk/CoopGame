// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
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

		//DrawDebugLine(GetWorld(), MuzzleLocation, traceEnd, FColor::White, false, 1.0f, 0, 1.0f);

		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);

		if(TracerEffect)
		{
			UParticleSystemComponent* tracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
			if(tracerComponent)
			{
				tracerComponent->SetVectorParameter("BeamEnd", tracerEnd);
			}
		}
	}
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

