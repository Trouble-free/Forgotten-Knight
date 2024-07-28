// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "Boss.h"

void UBossAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	BossRef = Cast<ABoss>(Pawn);
}

void UBossAnimInstance::UpdateAnimationProperties()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	if (!BossRef)
	{
		BossRef = Cast<ABoss>(Pawn);
	}

	if (BossRef)
	{
		FVector Speed = BossRef->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);
		MovementSpeed = LateralSpeed.Size();

		FRotator Rotation = BossRef->GetActorRotation();
		ControllerDir = CalculateDirection(Speed, Rotation);

		bIsAttackMode = BossRef->CurrentStatus == EBossStatus::EBoss_Attack;

	}

}
