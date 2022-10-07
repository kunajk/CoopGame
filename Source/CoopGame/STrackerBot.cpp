// Fill out your copyright notice in the Description page of Project Settings.


#include "STrackerBot.h"

#include "EngineUtils.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SCharacter.h"
#include "SHealthComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Public/TimerManager.h"

int32 DebugTrackerBotDrawing = 0;
FAutoConsoleVariableRef TrackerBot(TEXT("COOP.DebugTrackerBot"), DebugTrackerBotDrawing, TEXT("Draw Debug lines for TrackerBot"), ECVF_Cheat);

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

	if(GetLocalRole()==ROLE_Authority)
		NextPatchPoint = GetNextPatchPoint();

	FTimerHandle TimerHandle_OnCheckPowerLevel;
	return GetWorldTimerManager().SetTimer(TimerHandle_OnCheckPowerLevel, this, &ASTrackerBot::OnCheckNearbyBots, 1.0f, true);;
}

FVector ASTrackerBot::GetNextPatchPoint()
{
	if(!ensure(GetLocalRole()==ROLE_Authority))
		return FVector::Zero();
	
	ACharacter* myPawn = Cast<ACharacter>(GetClosestTarget());
	
	if(!IsValid(myPawn))
		return FVector::Zero();
	
	UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), myPawn);

	GetWorldTimerManager().ClearTimer(TimerHandle_RefreashPath);
	GetWorldTimerManager().SetTimer(TimerHandle_RefreashPath, this, &ASTrackerBot::RefreshPath, 5.0f, false);
	
	if(path && path->PathPoints.Num() > 1)
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
	UGameplayStatics::SpawnSoundAtLocation(this, ExplodeSound, GetActorLocation());

	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(HasAuthority())
	{
		TArray<AActor*> ignoredActors;
		ignoredActors.Add(this);
		float damage = BaseExplosionDamage + BaseExplosionDamage*PowerLevel;
		UGameplayStatics::ApplyRadialDamage(this, damage, GetActorLocation(), ExplosionRadius, nullptr, ignoredActors, this, GetInstigatorController(), true);
		SetLifeSpan(2.0f);
	}
	if(DebugTrackerBotDrawing)
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Red, false, 2.0f, 0,1.0f);
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, SelfDamageValue, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::OnCheckNearbyBots()
{
	const float DISTANCE_TO_CHECK = 600.0f;

	FCollisionShape collisionShape;
	collisionShape.SetSphere(DISTANCE_TO_CHECK);

	FCollisionObjectQueryParams params;
	params.AddObjectTypesToQuery(ECC_PhysicsBody);
	params.AddObjectTypesToQuery(ECC_Pawn);
	params.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	TArray<FOverlapResult> overlaps;
	GetWorld()->OverlapMultiByObjectType(overlaps, GetActorLocation(), FQuat::Identity, params, collisionShape);

	int32 numberOfBots = 0;
	for(auto& r : overlaps)
	{
		ASTrackerBot* bot = Cast<ASTrackerBot>(r.GetActor());
		if(bot && bot != this)
		{
			numberOfBots++;
		}
	}

	const int32 MAX_POWER_LEVEL = 4;
	PowerLevel = FMath::Clamp(numberOfBots, 0, MAX_POWER_LEVEL);

	if(!IsValid(MatInst))
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	if(IsValid(MatInst))
	{
		float alpha = PowerLevel/(float)MAX_POWER_LEVEL;
		MatInst->SetScalarParameterValue("PowerLevelAlpha", alpha);
	}
}

AActor* ASTrackerBot::GetClosestTarget() const
{
	ASCharacter* closestTarget{nullptr};
	float closestDistance = TNumericLimits<float>::Max();
	
	for(TActorIterator<ASCharacter> It(GetWorld()); It; ++It)
	{
		USHealthComponent* healthComp = Cast<USHealthComponent>(It->GetComponentByClass(USHealthComponent::StaticClass()));
		
		if(IsValid(healthComp) && (healthComp->GetHealth() > 0.0f))
		{
			if(It->IsPlayerControlled())
			{
				float distanceToBot = (GetActorLocation() - It->GetActorLocation()).Length();
				if(distanceToBot < closestDistance)
				{
					closestDistance = distanceToBot;
					closestTarget = *It;
				}
			}
		}
	}
	return closestTarget;
}

void ASTrackerBot::RefreshPath()
{
	NextPatchPoint = GetNextPatchPoint();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetLocalRole() != ROLE_Authority || bExploded)
		return;
	
	float distanceToTarget = (GetActorLocation() - NextPatchPoint).Size();
	if(distanceToTarget > RequirdDistanceToTarget)
	{
		FVector forceDirection = NextPatchPoint - GetActorLocation();
		forceDirection.Normalize();
		forceDirection *= MoveForce;
		MeshComp->AddForce(forceDirection, NAME_None, UseVelocityChange);

		if(DebugTrackerBotDrawing)
			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + forceDirection, 32.0f, FColor::Yellow);
	}
	else
	{
		NextPatchPoint = GetNextPatchPoint();

		if(DebugTrackerBotDrawing)
			DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Target reached!"));
	}
	if(DebugTrackerBotDrawing)
		DrawDebugSphere(GetWorld(), NextPatchPoint, 20.0f, 12, FColor::Blue, false, 0.0f, 1.0f);
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(bStartedSelfDestruction || bExploded)
		return;

	if(USHealthComponent::IsFriendly(this, OtherActor))
		return;
	
	ASCharacter* playerPawn = Cast<ASCharacter>(OtherActor);

	if(IsValid(playerPawn))
	{
		if(HasAuthority())
			GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, SelfDamageInterval, true, 0.0f);

		bStartedSelfDestruction = true;
		UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
	}
}
