// Copyright Epic Games, Inc. All Rights Reserved.

#include "FSMSysCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HealthComponent.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISense_Sight.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFSMSysCharacter

AFSMSysCharacter::AFSMSysCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	LockOnComp = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOnComp"));
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComponent"));

	// Register sight stimuli
	StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimuliSourceComponent->RegisterWithPerceptionSystem();
}

void AFSMSysCharacter::Attack_Implementation()
{
	AActor* Target = LockOnComp->GetLockOnTarget();
	if (Target)
	{
		float Damage = FMath::RandRange(MinDamage, MaxDamage);
		if (FVector::Dist(Target->GetActorLocation(), GetActorLocation()) <= AttackDist)
		{
			if (ICombatInterface* CombatTarget = Cast<ICombatInterface>(Target))
			{
				CombatTarget->TakeDamage(Damage, this);
			}
		}
	}
}

void AFSMSysCharacter::TakeDamage(float Damage, ICombatInterface* Attacker)
{
	if (bAlive)
	{
		LastAttacker = Attacker;
	}
	HealthComp->TakeDamage(Damage);
	if (!bAlive) return;
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (!HitMontage) return;
		AnimInst->Montage_Play(HitMontage);
		bCanAttack = false;
	}
}

void AFSMSysCharacter::Kill()
{
	if (!bAlive) return;
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (!KOMontage) return;
		UE_LOG(LogTemp, Display, TEXT("Playing KO Anim on Player"));
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

void AFSMSysCharacter::AddKillCount()
{
	Score++;
}

void AFSMSysCharacter::OnDeath()
{
	if (LastAttacker)
	{
		LastAttacker->AddKillCount();
	}
	Kill();
}

void AFSMSysCharacter::Ragdoll()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AFSMSysCharacter::Respawn, 3.f, false);
}

void AFSMSysCharacter::Respawn()
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
	if (AController* PlayerController = GetController())
	{
		PlayerController->EnableInput(nullptr);
	}
	
	GetCapsuleComponent()->SetWorldRotation(FRotator(0.0f, GetActorRotation().Yaw, 0.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, .0f));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	SetActorLocation(SpawnPoint);
}

void AFSMSysCharacter::Punch()
{
	if (!bCanAttack) return;
	if(auto* const AnimInst = GetMesh()->GetAnimInstance()){
		int32 RandIndex = FMath::RandRange(0, PunchMontages.Num() - 1);
		if (UAnimMontage* SelectedMont = PunchMontages[RandIndex])
		{
			AnimInst->Montage_Play(SelectedMont);
			bCanAttack = false;
		}
	}
}

void AFSMSysCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->OnDeath.AddDynamic(this, &AFSMSysCharacter::OnDeath);
	SpawnPoint = GetActorLocation();
}

void AFSMSysCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LockOnComp->GetLockOnTarget())
	{
		RotateToTarget(DeltaTime);
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

void AFSMSysCharacter::LockOnTarget()
{
	UE_LOG(LogTemp, Log, TEXT("Locking onto target"));
	LockOnComp->FindTarget();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 150;
}

void AFSMSysCharacter::UnlockOnTarget()
{
	UE_LOG(LogTemp, Log, TEXT("Target unlock"));
	LockOnComp->ClearTarget();
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 500;
}

void AFSMSysCharacter::RotateToTarget(float DeltaTime)
{
	AActor* CurrentTarget = LockOnComp->GetLockOnTarget();
	if (!CurrentTarget) return;

	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());
	FRotator NewRot = FMath::RInterpTo(GetActorRotation(), LookRot, DeltaTime, 5.f);

	SetActorRotation(NewRot);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFSMSysCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFSMSysCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFSMSysCharacter::Look);

		//Lock on
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &AFSMSysCharacter::LockOnTarget);
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Completed, this, &AFSMSysCharacter::UnlockOnTarget);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AFSMSysCharacter::Punch);
		
		//Quit
		EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Started, this, &AFSMSysCharacter::Quit);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFSMSysCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFSMSysCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFSMSysCharacter::Quit(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
}
bool AFSMSysCharacter::IsInCombat()
{
	if (!LockOnComp) return false;
	if (LockOnComp->GetLockOnTarget()){ return true; } else { return false;}
}
