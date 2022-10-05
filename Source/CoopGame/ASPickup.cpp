// Fill out your copyright notice in the Description page of Project Settings.


#include "ASPickup.h"

#include "SPowerUP.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AASPickup::AASPickup()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	SetRootComponent(SphereComp);
	
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90,0,0));
	DecalComp->DecalSize = FVector(64,75,75);
	DecalComp->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void AASPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(HasAuthority() && PowerupInstance)
	{
		PowerupInstance->ActivatePowerup(OtherActor);
		PowerupInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AASPickup::Respawn, CooldownDuration);
	}
}

// Called when the game starts or when spawned
void AASPickup::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
		Respawn();
}

void AASPickup::Respawn()
{
	if(PowerupClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Powerupclass is null in %s. Please update your blueprint"), *GetName());
		return;
	}
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PowerupInstance = GetWorld()->SpawnActor<ASPowerUP>(PowerupClass, GetTransform(), params); 
}


