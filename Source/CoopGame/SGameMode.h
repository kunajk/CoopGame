// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameMode();
	
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Coop|GameMode")
	void SpawnNewBot();

	UFUNCTION()
	void SpawnNewBotTimerElapsed();


	void StartWave();
	void EndWave();
	void PrepareForNextWave();
	void CheckWaveState();
	bool IsAnyPlayerAlive() const;
	void GameOver();
	
	UPROPERTY(EditDefaultsOnly, Category="Coop|Config")
	float TimeBetweenWaves{30.0f};
	
	UPROPERTY()
	FTimerHandle TimerHandle_BotSpawner;

	UPROPERTY()
	FTimerHandle TimerHandle_NextWaveStarts;
	
	UPROPERTY()
	int32 NumberOfBotsToSpawn;

	UPROPERTY()
	int32 WaveCount{0};

	UPROPERTY()
	bool bIsGameOver{false};
};
