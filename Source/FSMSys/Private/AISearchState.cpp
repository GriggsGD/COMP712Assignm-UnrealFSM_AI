// Fill out your copyright notice in the Description page of Project Settings.


#include "AISearchState.h"
#include "Runtime/AIModule/Classes/AIController.h"

void UAISearchState::EnterState_Implementation(AActor* OwnerActor)
{
	Super::EnterState_Implementation(OwnerActor);
	NPCCtrl->SetMoveSpeed(350.0f);
	NPCCtrl->MoveToPoint(NPCCtrl->GetLastPos());
}

void UAISearchState::UpdateState_Implementation(AActor* OwnerActor, float DeltaTime)
{
	Super::UpdateState_Implementation(OwnerActor, DeltaTime);

	if (FVector::Dist(NPCCtrl->GetLastPos(), OwnerActor->GetActorLocation()) < NPCCtrl->AcceptanceRadius)
	{
		NPCCtrl->GetStateMachine()->ChangeState(NPCCtrl->PatrolState);
	}
	
	if (NPCCtrl->GetSensedActor())
	{
		if (AAIController* AICtrl = Cast<AAIController>(NPCCtrl->GetController()))
		{
			AICtrl->StopMovement();
		}
		NPCCtrl->GetStateMachine()->ChangeState(NPCCtrl->ChaseState);
	}
}

void UAISearchState::ExitState_Implementation(AActor* OwnerActor)
{
	Super::ExitState_Implementation(OwnerActor);
}
