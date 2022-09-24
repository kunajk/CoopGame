// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComponent.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* myOwner = GetOwner();
	if(myOwner)
	{
		myOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
	}
	
	Health = DefaultHealth;;
}

void USHealthComponent::HandleDie()
{
	AActor* myOwner = GetOwner();
	//if(myOwner)
		//myOwner->Destroy();
}

void USHealthComponent::HandleTakeAnyDamage(AActor* Actor, float Damage, const UDamageType* DamageType, AController* Instigated,
                                            AActor* DamageCauser)
{
	if(Damage <= 0.0f)
		return;

	float newHealth = FMath::Clamp(Health-Damage, 0.0f, DefaultHealth);
	
	UE_LOG(LogTemp, Log, TEXT("Health changed from %0.2f to %0.2f, and sanitized float: %s"), Health, newHealth, *FString::SanitizeFloat(Health));

	Health = newHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, Instigated, DamageCauser);
	if(FMath::IsNearlyEqual(Health, 0.0f))
	{
		HandleDie();
	}
}


// Called every frame
void USHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

