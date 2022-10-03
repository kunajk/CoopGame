// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnHealthChanged);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComp->SetupAttachment(RootComponent);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	SetReplicates(true);
	AActor::SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::OnHealthChanged(USHealthComponent* HealthComponent, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* Instigated, AActor* DamageCauser)
{
	if(bExploded)
		return;

	if(Health <= 0.0f)
	{
		bExploded = true;
		OnRep_Exploded();
		FVector boostIntensivity = FVector::UpVector * ExplosionImpulse;
		MeshComp->AddImpulse(boostIntensivity, NAME_None, true);
		
		RadialForceComp->FireImpulse();

		TArray<AActor*> ignoredActors;
		ignoredActors.Add(this);
		UGameplayStatics::ApplyRadialDamage(this, 100, GetActorLocation(), 200, nullptr, ignoredActors, this, GetInstigatorController(), true);
	}
}

void ASExplosiveBarrel::OnRep_Exploded()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	MeshComp->SetMaterial(0, ExplodedMaterial);
}
void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASExplosiveBarrel, bExploded);
}
