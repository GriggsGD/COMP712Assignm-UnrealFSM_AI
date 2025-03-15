// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnComponent.h"

#include "StateDrivenNPC.h"
#include "Kismet/GameplayStatics.h"

ULockOnComponent::ULockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	LockOnTarget = nullptr;
}
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();
}
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
AActor* ULockOnComponent::FindClostestTarget()
{
	AActor* Owner = GetOwner();
	if (!Owner) return nullptr;

	TArray<AActor*> AllTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStateDrivenNPC::StaticClass(), AllTargets);

	AActor* ClostestTarget = nullptr;
	float ClosestDist = TNumericLimits<float>::Max();

	for (AActor* Target : AllTargets)
	{
		float Dist = FVector::Dist(Owner->GetActorLocation(), Target->GetActorLocation());
		if (Dist < ClosestDist)
		{
			ClosestDist = Dist;
			ClostestTarget = Target;
		}
	}
	return ClostestTarget;
}
void ULockOnComponent::FindTarget()
{
	LockOnTarget = FindClostestTarget();
}
void ULockOnComponent::ClearTarget()
{
	LockOnTarget = nullptr;
}

