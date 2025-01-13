// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrolState.h"
#include "StateDrivenNPC.h"

void UAIPatrolState::EnterState_Implementation(AActor* OwnerActor)
{
	Super::EnterState_Implementation(OwnerActor);
	
	if (NPCCtrl)
	{
		UE_LOG(LogTemp, Log, TEXT("Current patrol point: %i"), NPCCtrl->CurrentPatrolPointIndex);
		NPCCtrl->MoveToNextPatrolPoint();
	}
}

void UAIPatrolState::UpdateState_Implementation(AActor* OwnerActor, float const DeltaTime)
{
	Super::UpdateState_Implementation(OwnerActor, DeltaTime);
	if (!NPCCtrl || NPCCtrl->PatrolPoints.Num() == 0)
	{
		return;
	}

	float DistanceToTarget = FVector::Dist(NPCCtrl->GetActorLocation(), NPCCtrl->PatrolPoints[NPCCtrl->CurrentPatrolPointIndex]->GetActorLocation());
	if (DistanceToTarget < NPCCtrl->AcceptanceRadius)
	{
		NPCCtrl->CurrentPatrolPointIndex = (NPCCtrl->CurrentPatrolPointIndex + 1) % NPCCtrl->PatrolPoints.Num();
		NPCCtrl->GetStateMachine_Implementation()->ChangeState(NPCCtrl->AvailableStates[0]);
	}
}

void UAIPatrolState::ExitState_Implementation(AActor* OwnerActor)
{
	Super::ExitState_Implementation(OwnerActor);
}
