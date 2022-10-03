// Fill out your copyright notice in the Description page of Project Settings.


#include "STrackerBot.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SCharacter.h"
#include "SHealthComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Public/TimerManager.h"

// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	SetRootComponent(MeshComp);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);
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

void ASTrackerBot::HandleTakeDamage(USHealthComponent* UsHealthComponent, float Health, float HealthDelta, const UDamageType* Damage,
	AController* AiController, AActor* Actor)
{
	UE_LOG(LogTemp, Log, TEXT("Heallth %s of %s"), *FString::SanitizeFloat(Health), *GetName())
	
	if(!IsValid(MatInst))
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));

	if(IsValid(MatInst))
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);

	if(Health <= 0.0f)
		SelfDestruct();
}

void ASTrackerBot::SelfDestruct()
{
	if(bExploded)
		return;

	bExploded = true;
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	TArray<AActor*> ignoredActors;
	ignoredActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, ignoredActors, this, GetInstigatorController(), true);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Red, false, 2.0f, 0,1.0f);
	UGameplayStatics::SpawnSoundAtLocation(this, ExplodeSound, GetActorLocation());
	
	Destroy();
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, SelfDamageValue, GetInstigatorController(), this, nullptr);
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

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(bStartedSelfDestruction)
		return;
	
	ASCharacter* playerPawn = Cast<ASCharacter>(OtherActor);

	if(IsValid(playerPawn))
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, SelfDamageInterval, true, 0.0f);
		bStartedSelfDestruction = true;
		UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
	}
}
