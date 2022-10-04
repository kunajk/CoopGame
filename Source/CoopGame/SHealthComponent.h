// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"

class USHealthComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, USHealthComponent*, HealthComponent, float, Health, float, HealthDelta, const UDamageType*, DamageType, AController*, Instigated, AActor*, DamageCauser);

UCLASS(Blueprintable, ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHealthComponent();

	UPROPERTY(BlueprintAssignable, Category="Coop|Events")
	FOnHealthChangedSignature OnHealthChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void HandleDie();
	
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* Actor, float X, const UDamageType* Damage, AController* Controller, AActor* Actor1);

	UFUNCTION()
	void OnRep_Health(float oldHealth);
	
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Coop|HealthCOmponent")
	float Health{100.0f};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Coop|HealthCOmponent")
	float DefaultHealth{100.0f};
	
};
