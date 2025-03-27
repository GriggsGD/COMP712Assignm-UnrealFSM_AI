// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseState.h"
#include "GameFramework/Actor.h"
UBaseState::UBaseState()
	: ControllingStateMachine(nullptr)
{
}
void UBaseState::Initialize(UStateMachine* InStateMachine)
{
	ControllingStateMachine = InStateMachine;
}

UStateMachine* UBaseState::GetControllingStateMachine_Implementation() const
{
	return ControllingStateMachine;
}

void UBaseState::EnterState_Implementation(AActor* OwnerActor)
{
	// Default implementation (optional, can be overridden in derived classes)
	//UE_LOG(LogTemp, Log, TEXT("%s entered state: %s"), *OwnerActor->GetName(), *GetName());
}

void UBaseState::UpdateState_Implementation(AActor* OwnerActor, float DeltaTime)
{
	// Default implementation (optional, can be overridden in derived classes)
}

void UBaseState::ExitState_Implementation(AActor* OwnerActor)
{
	// Default implementation (optional, can be overridden in derived classes)
	//UE_LOG(LogTemp, Log, TEXT("%s exited state: %s"), *OwnerActor->GetName(), *GetName());
}

