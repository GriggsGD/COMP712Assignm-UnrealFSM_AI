// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentRewardsComponent.h"

#include "BaseCharacter.h"

// Sets default values for this component's properties
UAgentRewardsComponent::UAgentRewardsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	LastHealth = 0;
	LastScore = 0;
	HitsLanded = 0;
	HitsMissed = 0;
	WasLockedOn = false;
	IdleTime = 0;
	TimeAlive = 0;
	WasRewardedEnemyLowHealth = false;
}


void UAgentRewardsComponent::ResetStats()
{
	LastHealth = 0;
	LastScore = 0;
	HitsLanded = 0;
	HitsMissed = 0;
	WasLockedOn = false;
	IdleTime = 0;
	TimeAlive = 0;
	WasRewardedEnemyLowHealth = false;
}

// Called when the game starts
void UAgentRewardsComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UAgentRewardsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

