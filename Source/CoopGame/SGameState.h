// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

UENUM(BlueprintType)
enum class ESGameState : uint8
{
	WaitingToStart,
	WaveInProgress,
	WaitingToComplete,
	WaveComplete,
	GameOver
};

UCLASS()
class COOPGAME_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	void SetGameState(ESGameState NewGameState);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WaveState, Category="Coop|GameState")
	ESGameState State;

protected:
	virtual void GetLifetimeReplicatedProps(class TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent, Category="Coop|GameState")
	void OnGameStateChanged(ESGameState NewState, ESGameState OldState);
	
	UFUNCTION()
	void OnRep_WaveState(ESGameState PrevState);
};
