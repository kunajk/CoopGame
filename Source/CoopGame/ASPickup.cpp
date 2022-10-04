// Fill out your copyright notice in the Description page of Project Settings.


#include "ASPickup.h"

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
}

void AASPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

// Called when the game starts or when spawned
void AASPickup::BeginPlay()
{
	Super::BeginPlay();


	
}


