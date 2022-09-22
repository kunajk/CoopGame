// Fill out your copyright notice in the Description page of Project Settings.

#include "SProjectTile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASProjectTile::ASProjectTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->SetSimulatePhysics(true);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
}

// Called when the game starts or when spawned
void ASProjectTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASProjectTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

