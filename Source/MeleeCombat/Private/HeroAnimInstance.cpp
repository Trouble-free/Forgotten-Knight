// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "HeroCharacter.h"
#include "LockOnComponent.h"
#include "ComboSystem.h"

void UHeroAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}

	PlayerRef = Cast<AHeroCharacter>(Pawn);
}

void UHeroAnimInstance::UpdateAnimationProperties()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);
		MovementSpeed = LateralSpeed.Size();

		bIsJumping = Pawn->GetMovementComponent()->IsFalling();

		FRotator Rotation = Pawn->GetActorRotation();

		ControllerDir = CalculateDirection(Speed, Rotation);

		if (!PlayerRef)
		{
			PlayerRef = Cast<AHeroCharacter>(Pawn);
		}
		if (PlayerRef)
		{
			bIsLockOn = PlayerRef->GetLockOnComp()->bIsLockOnEngaged;
		}
	}
}
