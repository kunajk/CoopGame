// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShakeBase;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASWeapon();

	void StartFire();
	void StopFire();
protected:

	void BeginPlay() override;

	void PlayFireEffects(FVector& MuzzleLocation, FVector& TraceEnd);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coop|Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	FName MuzzleSocketName{"MuzzleSocket"};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	UParticleSystem* FlashImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	UParticleSystem* HitActorEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category="Coop|Weapon")
	TSubclassOf<UCameraShakeBase> FireShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	float BaseDamage{6.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	int32 BulletsPerMinute{600};
private:
	virtual void Fire();
	float LastTimeFired{0.0f};
	float TimeBetweenShots;
	FTimerHandle TimerHandle_TimeBetweenShots;
};
