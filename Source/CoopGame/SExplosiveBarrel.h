// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class USHealthComponent;
class UStaticMeshComponent;
class URadialForceComponent;
class UParticleSystem;
class UMaterialInterface;

UCLASS()
class COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* HealthComponent, float Health, float HealthDelta, const UDamageType* DamageType, AController* Instigated, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	USHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	URadialForceComponent* RadialForceComp;

	bool bExploded{false};

	UPROPERTY(EditDefaultsOnly, Category="Coop|FX")
	float ExplosionImpulse{400};

	UPROPERTY(EditDefaultsOnly, Category="Coop|FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category="Coop|FX")
	UMaterialInterface* ExplodedMaterial;
};
