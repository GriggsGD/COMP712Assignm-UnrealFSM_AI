// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, CurrHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FSMSYS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	void TakeDamage(float DamageAmount);
	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return CurrentHealth; }
	UFUNCTION(BlueprintCallable)
	float GetLastHealth() const { return LastHealth; }
	UFUNCTION(BlueprintCallable)
	void ResetLastHealth();
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintCallable)
	void Heal(float HealAmount);
	UFUNCTION(BlueprintCallable)
	void ResetHealth();
protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "0.0"))
	float MaxHealth = 150.0f;

	float CurrentHealth;
	float LastHealth;
};
