// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UStaticMeshComponent;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category="Coop|Components")
	UStaticMeshComponent* MeshComp;

	FVector GetNextPatchPoint();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	float MoveForce{1000.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	float RequirdDistanceToTarget{100};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coop")
	bool UseVelocityChange{false};
private:
	FVector NextPatchPoint; 
};
