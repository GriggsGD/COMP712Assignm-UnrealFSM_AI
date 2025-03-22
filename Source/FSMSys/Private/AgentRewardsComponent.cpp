// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentRewardsComponent.h"

#include "BaseCharacter.h"

// Sets default values for this component's properties
UAgentRewardsComponent::UAgentRewardsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAgentRewardsComponent::BeginPlay()
{
	Super::BeginPlay();
	HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();
	StaminaComp = GetOwner()->FindComponentByClass<UStaminaComponent>();
}


// Called every frame
void UAgentRewardsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

