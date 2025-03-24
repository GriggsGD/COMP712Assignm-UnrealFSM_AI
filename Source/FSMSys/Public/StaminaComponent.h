// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, CurrentStamina);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FSMSYS_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaChanged OnStaminaChanged;

	void UseStamina(float StamAmount);
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetStamina() const {return CurrStamina;}
	UFUNCTION(BlueprintCallable, Category="Stamina")
	float GetMaxStamina() const {return MaxStamina;}
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void AddStamina(float Stamina);
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void RegenTimeout();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void ResetStamina();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Stamina", meta=(ClampMin="0.0"))
	float MaxStamina = 150.f;
	UPROPERTY(EditAnywhere, Category = "Stamina", meta=(ClampMin="0.0"))
	float RegenValPerSec = 25.f;
	UPROPERTY(EditAnywhere, Category = "Stamina", meta=(ClampMin="0.0"))
	float RegenTimeoutTime = 1.75f;
	float RegenTimer;

	float CurrStamina;
};
