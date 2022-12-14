// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComponent.h"

#include "SGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	SetIsReplicatedByDefault(true);
}


bool USHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	if(!IsValid(ActorA) || !IsValid(ActorB))
		return true;
	
	USHealthComponent* healthCompA = Cast<USHealthComponent>(ActorA->GetComponentByClass(USHealthComponent::StaticClass()));
	USHealthComponent* healthCompB = Cast<USHealthComponent>(ActorB->GetComponentByClass(USHealthComponent::StaticClass()));

	if(!IsValid(healthCompA) || !IsValid(healthCompB))
		return true;

	return healthCompA->TeamNum == healthCompB->TeamNum;
}

void USHealthComponent::Heal(float HealthToHeal)
{
	if(HealthToHeal <= 0.0f || Health <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealthToHeal, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, -HealthToHeal, nullptr, nullptr, nullptr);
	UE_LOG(LogTemp, Log, TEXT("Health changed: %s (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealthToHeal));
}

float USHealthComponent::GetHealth() const
{
	return Health;
}

bool USHealthComponent::IsDead() const
{
	return bIsDead;
}

// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;;

	if(GetOwnerRole() != ROLE_Authority)
		return;
	
	AActor* myOwner = GetOwner();
	if(myOwner)
	{
		myOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
	}
}


void USHealthComponent::HandleDie(AActor* Killer, AController* InstigatedBy)
{
	ensure(bIsDead == false);
	
	bIsDead = true;
	
	AActor* myOwner = GetOwner();

	ASGameMode* gameMode = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
	if(IsValid(gameMode))
	{
		gameMode->OnActorKilled.Broadcast(GetOwner(), Killer, InstigatedBy);
	}
}

void USHealthComponent::HandleTakeAnyDamage(AActor* Actor, float Damage, const UDamageType* DamageType, AController* Instigated,
                                            AActor* DamageCauser)
{
	if(Damage <= 0.0f || bIsDead)
		return;

	if(DamageCauser != Actor && IsFriendly(DamageCauser, Actor))
		return;

	float newHealth = FMath::Clamp(Health-Damage, 0.0f, DefaultHealth);
	
	UE_LOG(LogTemp, Log, TEXT("Health changed from %0.2f to %0.2f, and sanitized float: %s"), Health, newHealth, *FString::SanitizeFloat(Health));

	Health = newHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, Instigated, DamageCauser);
	if(FMath::IsNearlyEqual(Health, 0.0f))
	{
		HandleDie(DamageCauser, Instigated);
	}
}

void USHealthComponent::OnRep_Health(float oldHealth)
{
	float damage = Health - oldHealth;
	OnHealthChanged.Broadcast(this, Health, damage, nullptr, nullptr, nullptr);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USHealthComponent, Health);
	DOREPLIFETIME(USHealthComponent, TeamNum);
}

