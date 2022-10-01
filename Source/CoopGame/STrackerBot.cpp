// Fill out your copyright notice in the Description page of Project Settings.


#include "STrackerBot.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	NextPatchPoint = GetNextPatchPoint();
	
}

FVector ASTrackerBot::GetNextPatchPoint()
{
	ACharacter* myPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), myPawn);
	if(path->PathPoints.Num() > 1)
	{
		return path->PathPoints[1];
	}
	return GetActorLocation();
		
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float distanceToTarget = (GetActorLocation() - NextPatchPoint).Size();
	if(distanceToTarget > RequirdDistanceToTarget)
	{
		FVector forceDirection = NextPatchPoint - GetActorLocation();
		forceDirection.Normalize();
		forceDirection *= MoveForce;
		MeshComp->AddForce(forceDirection, NAME_None, UseVelocityChange);

		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + forceDirection, 32.0f, FColor::Yellow);
	}
	else
	{
		NextPatchPoint = GetNextPatchPoint();
		DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Target reached!"));
	}
	DrawDebugSphere(GetWorld(), NextPatchPoint, 20.0f, 12, FColor::Blue, false, 0.0f, 1.0f);
}

