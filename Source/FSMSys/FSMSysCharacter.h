// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AgentRewardsComponent.h"
#include "BaseCharacter.h"
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
class AFSMSysCharacter : public ABaseCharacter
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

	virtual void ResetCharacter() override;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ResetAll();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void LogEpisodeResults();
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void MLMove(float XInput, float YInput);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void LookHoriOnly(float HoriLook);

	void Quit(const FInputActionValue& Value);
			
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInCombat();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Learning Agent")
	UAgentRewardsComponent* RewardsComponent;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "LockOn")
	ULockOnComponent* LockOnComp;

	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void LockOnTarget();
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void UnlockOnTarget();
	void RotateToTarget(float DeltaTime);

	UPROPERTY(EditAnywhere, Category="Combat", meta=(ClampMin="0.0"))
	float AttackDist = 125.f;

	UPROPERTY(EditAnywhere, Category="Combat", meta=(ClampMin="0.0"))
	float IdleVelocityThreshold = 0.5f;
};

