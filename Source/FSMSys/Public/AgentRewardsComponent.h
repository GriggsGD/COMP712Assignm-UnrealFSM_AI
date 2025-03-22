// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"
#include "Components/ActorComponent.h"
#include "AgentRewardsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FSMSYS_API UAgentRewardsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAgentRewardsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	float LastHealth;
	float LastStamina;
	int  HitsLanded;
	int HitMissed;
	bool WasLockedOn;
	float IdleTime;
	
	float TimeAlive;
	
	UPROPERTY()
	UHealthComponent* HealthComp;
	UPROPERTY()
	UStaminaComponent* StaminaComp;
};
