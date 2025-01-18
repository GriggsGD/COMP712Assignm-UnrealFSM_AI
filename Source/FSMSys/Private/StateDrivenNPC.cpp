// Fill out your copyright notice in the Description page of Project Settings.


#include "StateDrivenNPC.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "StateMachine.h"

// Sets default values
AStateDrivenNPC::AStateDrivenNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateMachine = CreateDefaultSubobject<UStateMachine>(TEXT("StateMachine"));
	
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	// Create and configure sight sense
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = SightFOV;
	SightConfig->SetMaxAge(StimulusMaxAge);
	SightConfig->DetectionByAffiliation.bDetectEnemies = DetectEnemies;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = DetectNeutrals;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = DetectFriendlies;

	// Add sight configuration to perception component
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// Bind to perception updates
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AStateDrivenNPC::OnPerceptionUpdated);
}

UStateMachine* AStateDrivenNPC::GetStateMachine() const
{
	return StateMachine;
}

AActor* AStateDrivenNPC::GetSensedActor()
{
	return SensedActor;
}

FVector AStateDrivenNPC::GetLastPos()
{
	return LastKnownPos;
}

void AStateDrivenNPC::SetIsInCombat(bool bInCombat)
{
	IsInCombat = bInCombat;
}

bool AStateDrivenNPC::GetIsInCombat() const
{
	return IsInCombat;
}

void AStateDrivenNPC::SetMoveSpeed(float Speed)
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = Speed;
	}
}

bool AStateDrivenNPC::GetCanAttack() const
{
	return bCanAttack;
}

void AStateDrivenNPC::Punch()
{
	UE_LOG(LogTemp, Log, TEXT("Punch"));
}

// Called when the game starts or when spawned
void AStateDrivenNPC::BeginPlay()
{
	Super::BeginPlay();
	InitializeStateMachine();
}

void AStateDrivenNPC::SetCanAttack(bool bNewCanAttack)
{
	bCanAttack = bNewCanAttack;
}

void AStateDrivenNPC::InitializeStateMachine()
{
	if (!StateMachine)
	{
		StateMachine = NewObject<UStateMachine>(this);
	}
	if (StateMachine)
	{
		StateMachine->Initialize(this, IdleState);
	}
}

void AStateDrivenNPC::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Log, TEXT("AI Detected Stimulus"));
		SensedActor = Actor;
	}
	else
	{
		SensedActor = nullptr;
	}
}

FVector AStateDrivenNPC::GetNavMeshPosition(FVector Pos) const
{
	FVector ActorLocation = Pos;
	FNavLocation NavMeshPosition;

	// Get the navigation system
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSystem)
	{
		if (NavSystem->ProjectPointToNavigation(ActorLocation, NavMeshPosition))
		{
			return NavMeshPosition.Location;
		}
	}
	return ActorLocation;
}

void AStateDrivenNPC::MoveToPoint(FVector Pos) const
{
	if(AAIController* AICtrl = Cast<AAIController>(GetController()))
	{
		AICtrl->MoveToLocation(Pos);
	}
}
// Called every frame
void AStateDrivenNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StateMachine->Update(DeltaTime);

	if (SensedActor)
	{
		LastKnownPos = GetNavMeshPosition(SensedActor->GetActorLocation());
	}
}

// Called to bind functionality to input
void AStateDrivenNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

