// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASPickup.generated.h"

class USphereComponent;
class UDecalComponent;
class ASPowerUP;

UCLASS()
class COOPGAME_API AASPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASPickup();
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Respawn();
	
	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category="Coop|Config")
	TSubclassOf<ASPowerUP> PowerupClass;

	UPROPERTY(EditDefaultsOnly, Category="Coop|Config")
	float CooldownDuration{0};

	FTimerHandle TimerHandle_RespawnTimer;
	
	UPROPERTY()
	ASPowerUP* PowerupInstance;
private:
};
