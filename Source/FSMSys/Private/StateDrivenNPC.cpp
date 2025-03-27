// Fill out your copyright notice in the Description page of Project Settings.


#include "StateDrivenNPC.h"

#include "HealthComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "StateMachine.h"
#include "Components/CapsuleComponent.h"
#include "LevelInstance/LevelInstanceTypes.h"

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
// Called when the game starts or when spawned
void AStateDrivenNPC::BeginPlay()
{
	Super::BeginPlay();
	InitializeStateMachine();
	SpawnPoint = PatrolPoints[FMath::RandRange(0, PatrolPoints.Num() - 1)]->GetActorLocation();
}
// Called every frame
void AStateDrivenNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StateMachine->Update(DeltaTime);

	if (SensedActor)
	{
		LastKnownPos = GetNavMeshPosition(SensedActor->GetActorLocation());
		if (ABaseCharacter* SensedCharacter = Cast<ABaseCharacter>(SensedActor))
		{
			if (!SensedCharacter->IsAlive()) SensedActor = nullptr;
		}
	}

	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (!bAlive) return;
		if (!AnimInst->IsAnyMontagePlaying())
		{
			bCanAttack = true;
		}
	}
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

void AStateDrivenNPC::Attack_Implementation()
{
	if (!bAlive) return;
	if (SensedActor)
	{
		float Damage = FMath::RandRange(MinDamage, MaxDamage);
		if (FVector::Dist(SensedActor->GetActorLocation(), GetActorLocation()) <= PunchDist)
		{
			if (ICombatInterface* CombatTarget = Cast<ICombatInterface>(SensedActor))
			{
				CombatTarget->TakeDamage(Damage, this);
			}
		}
	}
}

void AStateDrivenNPC::Ragdoll()
{
	SpawnPoint = PatrolPoints[FMath::RandRange(0, PatrolPoints.Num() - 1)]->GetActorLocation();
	Super::Ragdoll();
}

void AStateDrivenNPC::ResetCharacter()
{
	Super::ResetCharacter();
	GetStateMachine()->ChangeState(PatrolState);
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
		//UE_LOG(LogTemp, Log, TEXT("AI Detected Stimulus"));
		if (ABaseCharacter* SensedCharacter = Cast<ABaseCharacter>(Actor))
		{
			if (SensedCharacter->IsAlive()) { SensedActor = Actor; }
		}
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



