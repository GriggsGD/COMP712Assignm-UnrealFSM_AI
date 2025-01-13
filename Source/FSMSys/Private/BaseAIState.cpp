// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIState.h"

void UBaseAIState::EnterState_Implementation(AActor* OwnerActor)
{
	Super::EnterState_Implementation(OwnerActor);

	NPCCtrl = Cast<AStateDrivenNPC>(OwnerActor);
}
