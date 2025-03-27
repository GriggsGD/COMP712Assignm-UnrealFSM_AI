// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrolState.h"
#include "StateDrivenNPC.h"

void UAIPatrolState::EnterState_Implementation(AActor* OwnerActor)
{
	Super::EnterState_Implementation(OwnerActor);

	NPCCtrl->SetMoveSpeed(230.0f);
	
	if (NPCCtrl)
	{
		//UE_LOG(LogTemp, Log, TEXT("Current patrol point: %i"), NPCCtrl->CurrentPatrolPointIndex);

		if(NPCCtrl->PatrolPoints.Num() == 0) return;
		NPCCtrl->MoveToPoint(NPCCtrl->PatrolPoints[NPCCtrl->CurrentPatrolPointIndex]->GetActorLocation());
	}
}

void UAIPatrolState::UpdateState_Implementation(AActor* OwnerActor, float const DeltaTime)
{
	Super::UpdateState_Implementation(OwnerActor, DeltaTime);
	//Check if target is detected
	if (NPCCtrl->GetSensedActor() != nullptr)
	{
		NPCCtrl->GetStateMachine()->ChangeState(NPCCtrl->ChaseState);
	}
	
	if (!NPCCtrl || NPCCtrl->PatrolPoints.Num() == 0)
	{
		return;
	}

	float DistanceToTarget = FVector::Dist(NPCCtrl->GetActorLocation(), NPCCtrl->PatrolPoints[NPCCtrl->CurrentPatrolPointIndex]->GetActorLocation());
	if (DistanceToTarget < NPCCtrl->AcceptanceRadius)
	{
		NPCCtrl->CurrentPatrolPointIndex = (NPCCtrl->CurrentPatrolPointIndex + 1) % NPCCtrl->PatrolPoints.Num();
		NPCCtrl->GetStateMachine()->ChangeState(NPCCtrl->IdleState);
	}
}

void UAIPatrolState::ExitState_Implementation(AActor* OwnerActor)
{
	Super::ExitState_Implementation(OwnerActor);
}
