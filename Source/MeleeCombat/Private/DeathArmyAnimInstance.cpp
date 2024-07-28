// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathArmyAnimInstance.h"
#include "DeathArmy.h"

void UDeathArmyAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	EnemyRef = Cast<ADeathArmy>(Pawn);
}

void UDeathArmyAnimInstance::UpdateAnimationProperties()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	if (!EnemyRef)
	{
		EnemyRef = Cast<ADeathArmy>(Pawn);
	}
#if 1
	if (EnemyRef)
	{
		FVector Speed = EnemyRef->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);
		MovementSpeed = LateralSpeed.Size();

		FRotator Rotation = EnemyRef->GetActorRotation();
		ControllerDir = CalculateDirection(Speed, Rotation);

		bIsInBattle = EnemyRef->bIsInBattle;
	}
#endif
}
