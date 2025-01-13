// Fill out your copyright notice in the Description page of Project Settings.

#include "AIIdleState.h"
#include "GameFramework/Actor.h"
#include "StateDrivenNPC.h"
#include "Runtime/AIModule/Classes/AIController.h"

void UAIIdleState::EnterState_Implementation(AActor* OwnerActor)
{
	Super::EnterState_Implementation(OwnerActor);
	
	if (AAIController* AICtrl = Cast<AAIController>(NPCCtrl->GetController()))
	{
		AICtrl->StopMovement();
	}
	

	RandIdleTimer = FMath::RandRange(NPCCtrl->minIdleTime, NPCCtrl->maxIdleTime);
	GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, this, &UAIIdleState::Patrol, RandIdleTimer, false);
}

void UAIIdleState::UpdateState_Implementation(AActor* OwnerActor, float const DeltaTime)
{
	Super::UpdateState_Implementation(OwnerActor, DeltaTime);

	
}
void UAIIdleState::ExitState_Implementation(AActor* OwnerActor)
{
	Super::ExitState_Implementation(OwnerActor);
}

void UAIIdleState::Patrol() const
{
	NPCCtrl->GetStateMachine_Implementation()->ChangeState(NPCCtrl->AvailableStates[1]);
}
