// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"

#include "Net/UnrealNetwork.h"

void ASGameState::SetGameState(ESGameState NewGameState)
{
	if(HasAuthority())
	{
		ESGameState oldGameState = State;
		State = NewGameState;

		OnRep_WaveState(oldGameState);
	}
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGameState, State);
}

void ASGameState::OnRep_WaveState(ESGameState PrevState)
{
	OnGameStateChanged(State, PrevState);
}
