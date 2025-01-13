// Fill out your copyright notice in the Description page of Project Settings.


#include "StateDrivenNPC.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "StateMachine.h"

// Sets default values
AStateDrivenNPC::AStateDrivenNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StateMachine = NewObject<UStateMachine>(this);
}

UStateMachine* AStateDrivenNPC::GetStateMachine_Implementation()
{
	return StateMachine;
}

// Called when the game starts or when spawned
void AStateDrivenNPC::BeginPlay()
{
	Super::BeginPlay();
	InitializeStateMachine();
}
void AStateDrivenNPC::InitializeStateMachine()
{
	if (!StateMachine)
	{
		StateMachine = NewObject<UStateMachine>(this);
	}
	if (StateMachine)
	{
		// Load the initial state from the AvailableStates array
		if (AvailableStates.Num() > 0 && AvailableStates[0])
		{
			StateMachine->Initialize(this, AvailableStates[0]);
		}
	}
}

void AStateDrivenNPC::MoveToNextPatrolPoint()
{
	if(PatrolPoints.Num() == 0) return;

	if(AAIController* AICtrl = Cast<AAIController>(GetController()))
	{
		AICtrl->MoveToLocation(PatrolPoints[CurrentPatrolPointIndex]->GetActorLocation());
	}
}

// Called every frame
void AStateDrivenNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StateMachine->Update(DeltaTime);
}

// Called to bind functionality to input
void AStateDrivenNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

