// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"
#include "StateMachine.h"
#include "GameFramework/Character.h"
#include "StateMachineOwner.h"
#include "StateDrivenNPC.generated.h"

UCLASS()
class FSMSYS_API AStateDrivenNPC : public ACharacter, public IStateMachineOwner
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStateDrivenNPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//State Machine
	virtual UStateMachine* GetStateMachine_Implementation() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Machine")
	TArray<TSubclassOf<UBaseState>> AvailableStates;

	//Patrol
	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<AActor*> PatrolPoints;
	int32 CurrentPatrolPointIndex;

	void MoveToNextPatrolPoint();

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 100.0f;

	UPROPERTY(EditAnywhere)
	float minIdleTime = 1.5f;
	UPROPERTY(EditAnywhere)
	float maxIdleTime = 3.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	//State Machine
	UPROPERTY()
	UStateMachine* StateMachine;

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void InitializeStateMachine();
};
