// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"
#include "BaseState.h"
#include "Algo/Find.h"
#include "GameFramework/Actor.h"

UStateMachine::UStateMachine()
	: CurrentState(nullptr), PreviousState(nullptr), Owner(nullptr)
{
}

void UStateMachine::Initialize(AActor* OwnerActor, TSubclassOf<UBaseState> InitialStateClass)
{
	Owner = OwnerActor;
	if (InitialStateClass)
	{
		CurrentState = NewObject<UBaseState>(this, InitialStateClass);
		CurrentState->Initialize(this);
		StateInstances.Add(CurrentState);
		if (CurrentState)
		{
			CurrentState->EnterState(OwnerActor);
		}
	}
}

void UStateMachine::Update(float DeltaTime)
{
	if (CurrentState && Owner)
	{
		CurrentState->UpdateState(Owner, DeltaTime);
	}
}

void UStateMachine::ChangeState(TSubclassOf<UBaseState> NewStateClass)
{
	if (CurrentState)
	{
		CurrentState->ExitState(Owner);
		//UE_LOG(LogTemp, Log, TEXT("State Machine: Switching to State: %s"), *NewStateClass->GetName());
	}

	if (NewStateClass)
	{
		for (UBaseState* StateInstance : StateInstances)
		{
			if (StateInstance->GetClass() == NewStateClass)
			{
				CurrentState = StateInstance;
			}
		}
		if (CurrentState->GetClass() != NewStateClass)
		{
			CurrentState = NewObject<UBaseState>(this, NewStateClass);
			CurrentState->Initialize(this);
			StateInstances.Add(CurrentState);
		}
		
		if (CurrentState)
		{
			AActor* OwnerActor = CurrentState->GetOuter()->GetTypedOuter<AActor>();
			CurrentState->EnterState(OwnerActor);
		}
	}
}
