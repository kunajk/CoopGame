// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerUP.generated.h"

UCLASS()
class COOPGAME_API ASPowerUP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerUP();

	UFUNCTION(BlueprintImplementableEvent, Category="Coop|Powerups")
	void OnActivated();

	UFUNCTION(BlueprintImplementableEvent, Category="Coop|Powerups")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category="Coop|Powerups")
	void OnExpired();

	void ActivatePowerup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Coop|Powerups")
	float PowerUpInterval{0};

	UPROPERTY(EditDefaultsOnly, Category="Coop|Powerups")
	int32 TotalNumberOfTicks{0};

	UPROPERTY()
	int32 TickProcessed{0};
	
	UPROPERTY()
	FTimerHandle TimerHandle_PowerupTick;
private:
	UFUNCTION()
	void OnTickPowerup();

};
