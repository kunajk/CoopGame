// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();
protected:

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
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	UParticleSystem* HitActorEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Coop|Weapon")
	UParticleSystem* TracerEffect;
};
