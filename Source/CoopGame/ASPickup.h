// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASPickup.generated.h"

class USphereComponent;
class UDecalComponent;

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

	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category="Coop|Components")
	UDecalComponent* DecalComp;
private:
};
