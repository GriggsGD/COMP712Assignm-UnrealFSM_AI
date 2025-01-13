// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.generated.h"


UCLASS(Blueprintable, BlueprintType)
class UNREALSTATEMACHINE_API UBaseState : public UObject
{
	GENERATED_BODY()
public:
	UBaseState() {}
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void EnterState(AActor* OwnerActor);
	virtual void EnterState_Implementation(AActor* OwnerActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void UpdateState(AActor* OwnerActor, float DeltaTime);
	virtual void UpdateState_Implementation(AActor* OwnerActor, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void ExitState(AActor* OwnerActor);
	virtual void ExitState_Implementation(AActor* OwnerActor);
};
