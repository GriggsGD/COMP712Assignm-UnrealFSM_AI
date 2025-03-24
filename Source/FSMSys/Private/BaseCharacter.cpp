// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "StaminaComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	StaminaComp = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComp"));
}



// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->OnDeath.AddDynamic(this, &ABaseCharacter::OnDeath);
	SpawnPoint = GetActorLocation();
	TArray<AActor*> FoundCharas;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), FoundCharas);
	for(AActor* Chara : FoundCharas){
		if(Chara->GetActorLabel() != this->GetActorLabel()) {Opponent = Chara;}
	}
	UE_LOG(LogTemp, Log, TEXT("'%s' Opponent set to: '%s'"), *this->GetName(), *Opponent->GetName())
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (!bAlive) return;
		if (StaminaComp->GetStamina() < StaminaPerHit) { bCanAttack = false; }
		else if (!AnimInst->IsAnyMontagePlaying())
		{
			bCanAttack = true;
		}
	}
}
void ABaseCharacter::ResetCharacter()
{
	Score = 0;
	Respawn(SpawnPoint);
}
void ABaseCharacter::Attack_Implementation()
{
	ICombatInterface::Attack_Implementation();
}

void ABaseCharacter::TakeDamage(float Damage, ICombatInterface* Attacker)
{
	if (bAlive)
	{
		LastAttacker = Attacker;
	}
	HealthComp->TakeDamage(Damage);
	StaminaComp->RegenTimeout();
	if (!bAlive) return;
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (!HitMontage) return;
		AnimInst->Montage_Play(HitMontage);
		bCanAttack = false;
	}
}

void ABaseCharacter::Kill()
{
	if (!bAlive) return;
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (!KOMontage) return;
		AnimInst->Montage_Play(KOMontage);
		GetCharacterMovement()->DisableMovement();
		if (AController* PlayerController = GetController())
		{
			PlayerController->DisableInput(nullptr);
		}
		bCanAttack = false;
	}
	bAlive = false;
}

void ABaseCharacter::AddKillCount()
{
	Score++;
}

void ABaseCharacter::OnDeath()
{
	if (LastAttacker)
	{
		LastAttacker->AddKillCount();
	}
	Kill();
}

void ABaseCharacter::Ragdoll()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, [this]()
	{
		Respawn(SpawnPoint);
	}, 3.f, false);
}

void ABaseCharacter::Punch()
{
	if (!bCanAttack) return;
	if (PunchMontages.Num() <= 0) { UE_LOG(LogTemp, Error, TEXT("'%s' holds no Punch Montages!"), *GetNameSafe(this)); return; }
	if(auto* const AnimInst = GetMesh()->GetAnimInstance()){
		int32 RandIndex = FMath::RandRange(0, PunchMontages.Num() - 1);
		if (UAnimMontage* SelectedMont = PunchMontages[RandIndex])
		{
			AnimInst->Montage_Play(SelectedMont);
			StaminaComp->UseStamina(StaminaPerHit);
			bCanAttack = false;
		}
	}
}

void ABaseCharacter::Respawn(FVector SpawnPos)
{
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		AnimInst->StopAllMontages(0.f);
		bCanAttack = false;
	}
	
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	HealthComp->ResetHealth();
	StaminaComp->ResetStamina();
	bAlive = true;
	
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	if (AController* PlayerController = GetController())
	{
		PlayerController->EnableInput(nullptr);
	}
	
	GetCapsuleComponent()->SetWorldRotation(FRotator(0.0f, GetActorRotation().Yaw, 0.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, .0f));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	SetActorLocation(SpawnPos);
}



