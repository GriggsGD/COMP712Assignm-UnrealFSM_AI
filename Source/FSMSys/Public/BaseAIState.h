// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"
#include "StateDrivenNPC.h"
#include "BaseAIState.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FSMSYS_API UBaseAIState : public UBaseState
{
	GENERATED_BODY()

public:
	virtual void EnterState_Implementation(AActor* OwnerActor) override;

protected:
	UPROPERTY()
	AStateDrivenNPC* NPCCtrl;
};
