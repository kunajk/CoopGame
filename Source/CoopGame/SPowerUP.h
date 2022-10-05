// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerUP.generated.h"

class UPointLightComponent;
class UStaticMeshComponent;
class URotatingMovementComponent;
class USceneComponent;

UCLASS()
class COOPGAME_API ASPowerUP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerUP();
	virtual void GetLifetimeReplicatedProps(class TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Coop|Powerups")
	void OnActivated(AActor* ActivatedFor);

	UFUNCTION(BlueprintImplementableEvent, Category="Coop|Powerups")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category="Coop|Powerups")
	void OnExpired();

	void ActivatePowerup(AActor* ActivatedFor);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_PowerupActiv();

	UFUNCTION(BlueprintImplementableEvent, Category="Coop|Powerups")
	void ActivatePowerupStateChanged(bool bNewIsActive);
	
	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	USceneComponent* ScneeRootComp;
	
	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	UPointLightComponent* PointLightComp;

	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	URotatingMovementComponent* RotatingMovmentComp;

	UPROPERTY(EditDefaultsOnly, Category="Coop|Powerups")
	float PowerUpInterval{0};

	UPROPERTY(EditDefaultsOnly, Category="Coop|Powerups")
	int32 TotalNumberOfTicks{0};

	UPROPERTY()
	int32 TickProcessed{0};
	
	UPROPERTY()
	FTimerHandle TimerHandle_PowerupTick;

	UPROPERTY(ReplicatedUsing=OnRep_PowerupActiv)
	bool bIsPowerupActiv{false};
private:
	UFUNCTION()
	void OnTickPowerup();

};
