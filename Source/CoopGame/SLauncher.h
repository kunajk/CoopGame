// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SLauncher.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ASLauncher : public ASWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Coop")
	TSubclassOf<AActor> ProjectileClass;
protected:
	void Fire() override;
};
