// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatInterface.h"
#include "HealthComponent.h"
#include "LockOnComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Logging/LogMacros.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "FSMSysCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFSMSysCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Lock On Action **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnAction;
	
	/** Attack Action **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	
	/** Quit Action **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuitAction;

public:
	AFSMSysCharacter();
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual  void Attack_Implementation() override;
	virtual void TakeDamage(float Damage, ICombatInterface* Attacker) override;
	virtual void Kill() override;
	virtual void AddKillCount() override;
	UFUNCTION()
	void OnDeath();
	UFUNCTION(BlueprintCallable)
	void Ragdoll();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Punch();
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Quit(const FInputActionValue& Value);
			
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInCombat();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> PunchMontages;
	
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KOMontage;
private:
	UPROPERTY(VisibleAnywhere, Category = "LockOn")
	ULockOnComponent* LockOnComp;

	void LockOnTarget();
	void UnlockOnTarget();
	void RotateToTarget(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComp;
	
	bool bAlive = true;

	UPROPERTY(EditAnywhere, Category="Combat", meta=(ClampMin="0.0"))
	float AttackDist = 125.f;

	UPROPERTY(EditAnywhere, Category="Combat", meta=(ClampMin="0.0"))
	float MinDamage = 5.f;

	UPROPERTY(EditAnywhere, Category="Combat", meta=(ClampMin="0.0"))
	float MaxDamage = 20.f;

	UFUNCTION()
	void Respawn();
	FTimerHandle RespawnTimerHandle;
	FVector SpawnPoint;

	ICombatInterface* LastAttacker;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = "true"))
	int Score = 0;
};

