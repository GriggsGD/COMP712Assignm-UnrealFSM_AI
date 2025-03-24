// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"


UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrStamina = MaxStamina;
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RegenTimer > 0){ RegenTimer -= DeltaTime; }
	else
	{
		AddStamina(RegenValPerSec * DeltaTime);
	}
}

void UStaminaComponent::UseStamina(float StamAmount)
{
	if (CurrStamina <= 0) return;
	CurrStamina = FMath::Clamp(CurrStamina - StamAmount, 0.f, CurrStamina);
	RegenTimeout();
	OnStaminaChanged.Broadcast(CurrStamina);
}

void UStaminaComponent::AddStamina(float Stamina)
{
	if (CurrStamina >= MaxStamina) return;
	CurrStamina = FMath::Clamp(CurrStamina + Stamina, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrStamina);
}

void UStaminaComponent::RegenTimeout()
{
	RegenTimer = RegenTimeoutTime;
}

void UStaminaComponent::ResetStamina()
{
	CurrStamina = MaxStamina;
	OnStaminaChanged.Broadcast(CurrStamina);
}

