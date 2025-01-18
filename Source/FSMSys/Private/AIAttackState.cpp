// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAttackState.h"

void UAIAttackState::EnterState_Implementation(AActor* OwnerActor)
{
	Super::EnterState_Implementation(OwnerActor);
	NPCCtrl->SetMoveSpeed(115.0f);
	NPCCtrl->SetIsInCombat(true);
}

void UAIAttackState::UpdateState_Implementation(AActor* OwnerActor, float DeltaTime)
{
	Super::UpdateState_Implementation(OwnerActor, DeltaTime);

	if (NPCCtrl->GetSensedActor())
	{
		if (FVector::Dist(NPCCtrl->GetActorLocation(), NPCCtrl->GetSensedActor()->GetActorLocation()) > NPCCtrl->PunchDist)
		{
			NPCCtrl->SetCanAttack(false);
			NPCCtrl->MoveToPoint(NPCCtrl->GetSensedActor()->GetActorLocation());
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("Can attack"));
			NPCCtrl->SetCanAttack(true);
		}
		if (FVector::Dist(NPCCtrl->GetActorLocation(), NPCCtrl->GetSensedActor()->GetActorLocation()) > NPCCtrl->AttackDist)
		{
			NPCCtrl->GetStateMachine()->ChangeState(NPCCtrl->ChaseState);
		}
	}
	else
	{
		NPCCtrl->GetStateMachine()->ChangeState(NPCCtrl->SearchState);
	}
}

void UAIAttackState::ExitState_Implementation(AActor* OwnerActor)
{
	Super::ExitState_Implementation(OwnerActor);
	NPCCtrl->SetIsInCombat(false);
}
