// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"
#include "StateMachine.generated.h"

UCLASS(Blueprintable)
class UNREALSTATEMACHINE_API UStateMachine : public UObject
{
	GENERATED_BODY()
public:
	UStateMachine();
	
	UPROPERTY(Transient, VisibleAnywhere, Category = "State Machine")
	TArray<UBaseState*> StateInstances;

	UFUNCTION(BlueprintCallable, Category = "StateMachine")
	void Update(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void Initialize(AActor* OwnerActor, TSubclassOf<UBaseState> InitialStateClass);

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void ChangeState(TSubclassOf<UBaseState> NewStateClass);

private:

	UPROPERTY()
	TSubclassOf<UBaseState> CurrentStateClass;
	
	UPROPERTY()
	UBaseState* CurrentState;
	
	UPROPERTY()
	TSubclassOf<UBaseState> PreviousState;

	UPROPERTY()
	AActor* Owner;
};
