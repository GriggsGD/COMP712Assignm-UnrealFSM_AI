// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatInterface.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class FSMSYS_API ABaseCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual  void Attack_Implementation() override;
	virtual void TakeDamage(float Damage, ICombatInterface* Attacker) override;
	virtual void Kill() override;
	virtual void AddKillCount() override;

	UFUNCTION()
	virtual void OnDeath();
	UFUNCTION(BlueprintCallable)
	virtual  void Ragdoll();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Punch();

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void Respawn(FVector SpawnPos);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual bool IsAlive() { return bAlive; }
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual AActor* GetOpponent() {return Opponent;}

	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void ResetCharacter();

	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual int GetScore() {return Score;}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	UStaminaComponent* StaminaComp;
	
	bool bAlive = true;
	FTimerHandle RespawnTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	FVector SpawnPoint;
	
	UPROPERTY(EditAnywhere, Category="Combat", meta=(ClampMin="0.0"))
	float MinDamage = 5.f;

	UPROPERTY(EditAnywhere, Category="Combat", meta=(ClampMin="0.0"))
	float MaxDamage = 20.f;

	UPROPERTY(EditAnywhere, Category="Combat", meta=(ClampMin="0.0"))
	float StaminaPerHit = 25.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bCanAttack = true;
	
	ICombatInterface* LastAttacker;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = "true"))
	int Score = 0;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> PunchMontages;
	
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KOMontage;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AActor* Opponent;
};
