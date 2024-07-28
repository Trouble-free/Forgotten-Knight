// Fill out your copyright notice in the Description page of Project Settings.


#include "NonCollisionNotifyState.h"
#include "HeroCharacter.h"
#include "Components/CapsuleComponent.h"

void UNonCollisionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AHeroCharacter* const Hero = Cast<AHeroCharacter>(MeshComp->GetOwner()))
		{
			Hero->GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
			Hero->GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}
}

void UNonCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AHeroCharacter* const Hero = Cast<AHeroCharacter>(MeshComp->GetOwner()))
		{
			Hero->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
			Hero->GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
		}
	}
}
