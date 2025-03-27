// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseState.h"
#include "CombatInterface.h"
#include "HealthComponent.h"
#include "StateMachine.h"
#include "GameFramework/Character.h"
#include "StateMachineOwner.h"
#include "Animation/AnimMontage.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "StateDrivenNPC.generated.h"

UCLASS()
class FSMSYS_API AStateDrivenNPC : public ABaseCharacter, public IStateMachineOwner
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStateDrivenNPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//State Machine
	virtual UStateMachine* GetStateMachine() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|States")
	TSubclassOf<UBaseState> IdleState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|States")
	TSubclassOf<UBaseState> PatrolState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|States")
	TSubclassOf<UBaseState> ChaseState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|States")
	TSubclassOf<UBaseState> SearchState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|States")
	TSubclassOf<UBaseState> AttackState;
	
	//Patrol
	UPROPERTY(EditAnywhere, Category = "AI|Patrol")
	TArray<AActor*> PatrolPoints;
	int32 CurrentPatrolPointIndex;

	void MoveToPoint(FVector Pos) const;

	UPROPERTY(EditAnywhere, Category = "AI|Movement")
	float AcceptanceRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Idle")
	float minIdleTime = 1.5f;
	UPROPERTY(EditAnywhere, Category = "AI|Idle")
	float maxIdleTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float SightRadius = 1500.0f;
	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float LoseSightRadius = 1700.0f;
	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float SightFOV = 90.0f;
	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float StimulusMaxAge = 3.0f;
	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	bool DetectEnemies = true;
	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	bool DetectNeutrals = true;
	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	bool DetectFriendlies = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackDist = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float PunchDist = 125.0f;
	
	UFUNCTION(BlueprintCallable)
	AActor* GetSensedActor();
	UFUNCTION(BlueprintCallable)
	FVector GetLastPos();

	void SetIsInCombat(bool bInCombat);
	UFUNCTION(BlueprintCallable)
	bool GetIsInCombat() const;

	void SetMoveSpeed(float Speed);

	virtual void Attack_Implementation() override;

	virtual void Ragdoll() override;

	virtual void ResetCharacter() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool IsInCombat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool IsSearching;

private:
	//State Machine
	UPROPERTY()
	UStateMachine* StateMachine;

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void InitializeStateMachine();

	// Perception Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* PerceptionComponent;

	// Sight Configuration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Sight* SightConfig;

	// Callback for perception updates
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY()
	AActor* SensedActor;

	UPROPERTY()
	FVector LastKnownPos;

	FVector GetNavMeshPosition(FVector Pos) const;
	
};
