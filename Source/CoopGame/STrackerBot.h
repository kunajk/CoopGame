// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UStaticMeshComponent;
class USHealthComponent;
class UMaterialInstanceDynamic;
class UParticleSystem;
class USphereComponent;
class USoundBase;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category="Coop|Components")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Coop|Components")
	USHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, Category="Coop|Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	UParticleSystem* ExplosionEffect;

	FVector GetNextPatchPoint();
	
	UFUNCTION()
	void HandleTakeDamage(USHealthComponent* UsHealthComponent, float X, float Arg, const UDamageType* Damage, AController* AiController, AActor* Actor);

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	float MoveForce{1000.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	float RequirdDistanceToTarget{100};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	bool UseVelocityChange{false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	float ExplosionRadius{200};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	float ExplosionDamage{100};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	float SelfDamageInterval{0.25};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	float SelfDamageValue{5};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop|Sounds")
	USoundBase* SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop|Sounds")
	USoundBase* ExplodeSound;
private:
	void SelfDestruct();
	void DamageSelf();
	
	FVector NextPatchPoint;
	bool bExploded{false};
	bool bStartedSelfDestruction{false};

	UPROPERTY()
	UMaterialInstanceDynamic* MatInst;

	FTimerHandle TimerHandle_SelfDamage;
	
};

