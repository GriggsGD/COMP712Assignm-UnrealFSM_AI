// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIState.h"
#include "AIIdleState.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FSMSYS_API UAIIdleState : public UBaseAIState
{
	GENERATED_BODY()

public:
	
	virtual void EnterState_Implementation(AActor* OwnerActor) override;
	virtual void UpdateState_Implementation(AActor* OwnerActor, float DeltaTime) override;
	virtual void ExitState_Implementation(AActor* OwnerActor) override;
	
	void Patrol() const;
	
private:
	FTimerHandle IdleTimerHandle;
	float RandIdleTimer;
};
