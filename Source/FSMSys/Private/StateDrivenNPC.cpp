// Fill out your copyright notice in the Description page of Project Settings.


#include "StateDrivenNPC.h"

#include "HealthComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "StateMachine.h"
#include "Components/CapsuleComponent.h"

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

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
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

void AStateDrivenNPC::Punch()
{
	if (!bCanAttack) return;
	if (PunchMontages.Num() <= 0) { UE_LOG(LogTemp, Error, TEXT("NPC holds no Punch Montages!")); return; }
	if(auto* const AnimInst = GetMesh()->GetAnimInstance()){
		int32 RandIndex = FMath::RandRange(0, PunchMontages.Num() - 1);
		if (UAnimMontage* SelectedMont = PunchMontages[RandIndex])
		{
			AnimInst->Montage_Play(SelectedMont);
			bCanAttack = false;
		}
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
				CombatTarget->TakeDamage(Damage);
			}
		}
	}
}

void AStateDrivenNPC::TakeDamage(float DamageAmount)
{
	if (!bAlive) return;

	HealthComp->TakeDamage(DamageAmount);
	
	if (!bAlive) return;
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (!HitMontage) return;
		AnimInst->Montage_Play(HitMontage);
		bCanAttack = false;
	}
}

void AStateDrivenNPC::Kill()
{
	if (!bAlive) return;
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (!KOMontage) return;
		AnimInst->Montage_Play(KOMontage);
		GetCharacterMovement()->DisableMovement();
		bCanAttack = false;
	}
	bAlive = false;
}

void AStateDrivenNPC::Ragdoll()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AStateDrivenNPC::Respawn, 3.f, false);
}

void AStateDrivenNPC::OnDeath()
{
	Kill();
}
void AStateDrivenNPC::Respawn()
{
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		AnimInst->StopAllMontages(0.f);
		bCanAttack = false;
	}
	
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	HealthComp->Heal(HealthComp->GetMaxHealth());
	bAlive = true;
	
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	StateMachine->ChangeState(PatrolState);
	
	GetCapsuleComponent()->SetWorldRotation(FRotator(0.0f, GetActorRotation().Yaw, 0.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, .0f));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	SetActorLocation(PatrolPoints[FMath::RandRange(0, PatrolPoints.Num() - 1)]->GetActorLocation());
}


// Called when the game starts or when spawned
void AStateDrivenNPC::BeginPlay()
{
	Super::BeginPlay();
	InitializeStateMachine();
	HealthComp->OnDeath.AddDynamic(this, &AStateDrivenNPC::OnDeath);
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

	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (!bAlive) return;
		if (!AnimInst->IsAnyMontagePlaying())
		{
			bCanAttack = true;
		}
	}
}

// Called to bind functionality to input
void AStateDrivenNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

