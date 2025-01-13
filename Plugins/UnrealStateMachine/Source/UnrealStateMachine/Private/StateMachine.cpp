// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"
#include "BaseState.h"
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
		AActor* OwnerActor = CurrentState->GetOuter()->GetTypedOuter<AActor>();
		CurrentState->ExitState(OwnerActor);
		UE_LOG(LogTemp, Log, TEXT("State Machine: Switching to State: %s"), *CurrentState->GetClass()->GetName());
	}

	if (NewStateClass)
	{
		CurrentState = NewObject<UBaseState>(this, NewStateClass);
		if (CurrentState)
		{
			AActor* OwnerActor = CurrentState->GetOuter()->GetTypedOuter<AActor>();
			CurrentState->EnterState(OwnerActor);
		}
	}
}
