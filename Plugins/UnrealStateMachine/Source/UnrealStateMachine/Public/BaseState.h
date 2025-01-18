// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.generated.h"


class UStateMachine;

UCLASS(Blueprintable, BlueprintType)
class UNREALSTATEMACHINE_API UBaseState : public UObject
{
	GENERATED_BODY()
public:
	UBaseState();

	void Initialize(UStateMachine* InStateMachine);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State Machine")
	UStateMachine* GetControllingStateMachine() const;
	virtual UStateMachine* GetControllingStateMachine_Implementation() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Machine")
	void EnterState(AActor* OwnerActor);
	virtual void EnterState_Implementation(AActor* OwnerActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Machine")
	void UpdateState(AActor* OwnerActor, float DeltaTime);
	virtual void UpdateState_Implementation(AActor* OwnerActor, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Machine")
	void ExitState(AActor* OwnerActor);
	virtual void ExitState_Implementation(AActor* OwnerActor);
	
protected:
	UPROPERTY()
	UStateMachine* ControllingStateMachine;
};
