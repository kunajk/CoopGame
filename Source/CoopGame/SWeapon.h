// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShakeBase;

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;
	
	UPROPERTY()
	FVector_NetQuantize	TraceFrom;

	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

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
	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint, FVector TraceFrom);
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon", meta=(ClampMin=0.0f))
	float BulletsSpreadDegrees{2.0f};
private:
	virtual void Fire();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	
	float LastTimeFired{0.0f};
	float TimeBetweenShots;
	FTimerHandle TimerHandle_TimeBetweenShots;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();
};
