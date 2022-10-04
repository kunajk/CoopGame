// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUP.h"

// Sets default values
ASPowerUP::ASPowerUP()
{

}

void ASPowerUP::ActivatePowerup()
{
	if(PowerUpInterval > 0)
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerUP::OnTickPowerup, PowerUpInterval, true, 0);
	else
	{
		OnTickPowerup();
	}
}

// Called when the game starts or when spawned
void ASPowerUP::BeginPlay()
{
	Super::BeginPlay();

	
}

void ASPowerUP::OnTickPowerup()
{
	TickProcessed++;
	OnPowerupTicked();
	
	if(TickProcessed >= TotalNumberOfTicks)
	{
		OnExpired();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}
