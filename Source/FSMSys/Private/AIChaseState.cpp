// Fill out your copyright notice in the Description page of Project Settings.


#include "AIChaseState.h"

#include "StateDrivenNPC.h"
#include "Runtime/AIModule/Classes/AIController.h"

void UAIChaseState::EnterState_Implementation(AActor* OwnerActor)
{
	Super::EnterState_Implementation(OwnerActor);
	NPCCtrl->SetMoveSpeed(500.0f);
}

void UAIChaseState::UpdateState_Implementation(AActor* OwnerActor, float DeltaTime)
{
	Super::UpdateState_Implementation(OwnerActor, DeltaTime);

	if (NPCCtrl)
	{
		if (NPCCtrl->GetSensedActor())
		{
			NPCCtrl->MoveToPoint(NPCCtrl->GetSensedActor()->GetActorLocation());

			if (FVector::Dist(NPCCtrl->GetActorLocation(), NPCCtrl->GetSensedActor()->GetActorLocation()) < NPCCtrl->AttackDist)
			{
				if (AAIController* AICtrl = Cast<AAIController>(NPCCtrl->GetController()))
				{
					AICtrl->StopMovement();
				}
				NPCCtrl->GetStateMachine()->ChangeState(NPCCtrl->AttackState);
			}
		}
		else
		{
			NPCCtrl->GetStateMachine()->ChangeState(NPCCtrl->SearchState);
		}
	}
}

void UAIChaseState::ExitState_Implementation(AActor* OwnerActor)
{
	Super::ExitState_Implementation(OwnerActor);
}
