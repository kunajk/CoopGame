// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"

#include "EngineUtils.h"
#include "SCharacter.h"
#include "SHealthComponent.h"
#include "STrackerBot.h"

ASGameMode::ASGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();
	
	PrepareForNextWave();

	FTimerHandle TimerHandle_CheckingBotState;
	GetWorldTimerManager().SetTimer(TimerHandle_CheckingBotState, this, &ASGameMode::CheckWaveState, 1.0f, true);
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!bIsGameOver && IsAnyPlayerAlive() == false)
	{
		GameOver();
	}
}

void ASGameMode::SpawnNewBotTimerElapsed()
{
	SpawnNewBot();
	NumberOfBotsToSpawn--;
	
	if(NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ASGameMode::StartWave()
{
	WaveCount++;
	NumberOfBotsToSpawn = 2 + 20*WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnNewBotTimerElapsed, 1.0f, true, 0.0f);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStarts, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
}

void ASGameMode::CheckWaveState()
{
	if(NumberOfBotsToSpawn > 0)
		return;

	if(GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStarts))
	{
		return;
	}
	
	bool isAllBotsDead = true;
	for(TActorIterator<ASTrackerBot> It(GetWorld()); It; ++It)
	{
		USHealthComponent* healthComp = Cast<USHealthComponent>(It->GetComponentByClass(USHealthComponent::StaticClass()));
		
		isAllBotsDead = IsValid(healthComp) ? (healthComp->GetHealth() <= 0.0f) : true;

		if(isAllBotsDead == false)
		{
			break;
		}
	}

	if(isAllBotsDead)
		PrepareForNextWave();
}

bool ASGameMode::IsAnyPlayerAlive() const
{
	for(TActorIterator<ASCharacter> It(GetWorld()); It; ++It)
	{
		if(!It->IsPlayerControlled())
			continue;
		
		USHealthComponent* healthComp = Cast<USHealthComponent>(It->GetComponentByClass(USHealthComponent::StaticClass()));
		if(ensure(healthComp) && healthComp->GetHealth() > 0.0f)
			return true;
	}

	return false;
}

void ASGameMode::GameOver()
{
	if(bIsGameOver)
		return;

	bIsGameOver = true;
	
	EndWave();
	UE_LOG(LogTemp, Log, TEXT("GameOver, All player died"));
}
