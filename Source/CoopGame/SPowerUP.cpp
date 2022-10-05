// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUP.h"

#include "Components/PointLightComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerUP::ASPowerUP()
{
	ScneeRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComp"));
	SetRootComponent(ScneeRootComp);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetRelativeLocation(FVector(0,0,30));
	MeshComp->SetupAttachment(ScneeRootComp);
	MeshComp->bCastDynamicShadow = false;
	MeshComp->bCastStaticShadow = false;
	MeshComp->SetCastShadow(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComp"));
	PointLightComp->SetupAttachment(MeshComp);
	PointLightComp->SetRelativeLocation(FVector(0,0,30));
	PointLightComp->SetAttenuationRadius(200.0f);

	RotatingMovmentComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComp"));

	SetReplicates(true);
}

void ASPowerUP::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerUP, bIsPowerupActiv);
}

void ASPowerUP::ActivatePowerup(AActor* ActivatedFor)
{
	OnActivated(ActivatedFor);
	
	bIsPowerupActiv = true;
	OnRep_PowerupActiv();
	
	if(PowerUpInterval > 0)
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerUP::OnTickPowerup, PowerUpInterval, true);
	else
	{
		OnTickPowerup();
	}
}

// Called when the game starts or when spawned
void ASPowerUP::BeginPlay()
{
	Super::BeginPlay();

	
}

void ASPowerUP::OnRep_PowerupActiv()
{
	if(bIsPowerupActiv)
	{
		MeshComp->SetVisibility(false, true);
	}
	else
	{
		SetLifeSpan(2.0f);
	}
	
	ActivatePowerupStateChanged(bIsPowerupActiv);
}

void ASPowerUP::OnTickPowerup()
{
	TickProcessed++;
	OnPowerupTicked();
	
	if(TickProcessed >= TotalNumberOfTicks)
	{
		OnExpired();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);

		bIsPowerupActiv = false;
		OnRep_PowerupActiv();
	}
}
