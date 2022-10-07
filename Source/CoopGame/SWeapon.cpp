// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "CoopGame.h"
#include "GameplayDebuggerTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef DebugWeapon(TEXT("COOP.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug lines for weapons"), ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
	
	SetReplicates(true);
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}

void ASWeapon::Fire()
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}
	
	AActor* myOwner = GetOwner();
	if(myOwner)
	{
		FVector eyeLoc;
		FRotator eyeRot;
		myOwner->GetActorEyesViewPoint(eyeLoc, eyeRot);

		FVector shootDirection = eyeRot.Vector();
		FVector tracerEnd = eyeLoc + (shootDirection * 10000.0f);
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		float halfRad = FMath::DegreesToRadians(BulletsSpreadDegrees);
		shootDirection = FMath::VRandCone(shootDirection, halfRad, halfRad);
		
		FCollisionQueryParams params;
		params.bTraceComplex = true;
		params.bReturnPhysicalMaterial = true;
		
		
		FHitResult hit;
		EPhysicalSurface surfaceType = SurfaceType_Default;
		bool wasHit = GetWorld()->LineTraceSingleByChannel(hit, MuzzleLocation, tracerEnd, ECollisionChannel::COLLISION_WEAPON, params);
		if(wasHit)
		{
			tracerEnd = hit.ImpactPoint;
			surfaceType = UPhysicalMaterial::DetermineSurfaceType(hit.PhysMaterial.Get());
			
			float actualDamage = BaseDamage;
			if(surfaceType == SURFACE_FLESHVULNERABLE)
			{
				actualDamage *= 4.0f;
			}
			
			AActor* hitActor = hit.GetActor();
			if(IsValid(hitActor))
			{
				UGameplayStatics::ApplyPointDamage(hitActor, actualDamage, shootDirection, hit, myOwner->GetInstigatorController(), myOwner, DamageType);
			}
			
			PlayImpactEffects(surfaceType, hit.ImpactPoint, MuzzleLocation);

		}

		if(DebugWeaponDrawing > 0)
			DrawDebugLine(GetWorld(), MuzzleLocation, tracerEnd, FColor::White, false, 1.0f, 0, 1.0f);

		PlayFireEffects(MuzzleLocation, tracerEnd);

		if(GetLocalRole() == ROLE_Authority)
		{
			HitScanTrace.TraceFrom = MuzzleLocation;
			HitScanTrace.TraceTo = tracerEnd;
			HitScanTrace.SurfaceType = surfaceType;
		}
	}

	LastTimeFired = GetWorld()->TimeSeconds;
}

void ASWeapon::OnRep_HitScanTrace()
{
	//PlayCosmeticEffects
	PlayFireEffects(HitScanTrace.TraceFrom, HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo, HitScanTrace.TraceFrom);
}

void ASWeapon::StartFire()
{
	float firstDelay = FMath::Max(LastTimeFired - GetWorld()->GetTimeSeconds() + TimeBetweenShots, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true, firstDelay);
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer((TimerHandle_TimeBetweenShots));
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots =  60.0f/BulletsPerMinute;
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

void ASWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint, FVector TraceFrom)
{
	UParticleSystem* selectedParticle;
	switch(SurfaceType)
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
		FVector shotDirection = ImpactPoint - TraceFrom;
		shotDirection.Normalize();
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectedParticle, ImpactPoint, shotDirection.Rotation());
	}
}


void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
}
