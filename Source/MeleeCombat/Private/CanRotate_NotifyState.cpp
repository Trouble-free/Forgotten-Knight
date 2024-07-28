// Fill out your copyright notice in the Description page of Project Settings.


#include "CanRotate_NotifyState.h"
#include "HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCanRotate_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AHeroCharacter* Player = Cast<AHeroCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			// Allow player rotate before attack the enemy
			Player->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
		}
	}
}

void UCanRotate_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AHeroCharacter* Player = Cast<AHeroCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			// disable rotate when attack
			Player->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
		}
	}
}
