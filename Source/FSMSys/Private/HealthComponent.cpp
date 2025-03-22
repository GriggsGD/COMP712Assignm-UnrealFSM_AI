// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHealth = MaxHealth;
	LastHealth = CurrentHealth;
}


void UHealthComponent::TakeDamage(float DamageAmount)
{
	if (CurrentHealth <= 0) return;
	LastHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);

	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
	}
}

void UHealthComponent::ResetLastHealth()
{
	LastHealth = CurrentHealth;
}

void UHealthComponent::Heal(float HealAmount)
{
	if (CurrentHealth >= MaxHealth) return;
	LastHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);
	
}

void UHealthComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;
	LastHealth = CurrentHealth;
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

