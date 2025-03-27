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

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Rewards")
	float LastHealth;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Rewards")
	int  LastScore;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Rewards")
	int  HitsLanded;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Rewards")
	int HitsMissed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Rewards")
	bool WasLockedOn;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Rewards")
	float IdleTime;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Rewards")
	float TimeAlive;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Rewards")
	bool WasRewardedEnemyLowHealth;
	
	void ResetStats();
protected:
	virtual void BeginPlay() override;

private:

};
