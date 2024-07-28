// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingNotify.h"
#include "Attacker.h"
#include "RotatingComponent.h"

void URotatingNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		IAttacker* Attacker = Cast<IAttacker>(MeshComp->GetOwner());
		if (Attacker)
		{
			Attacker->ShouldRotateOn();
		}
	}
}

void URotatingNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		IAttacker* Attacker = Cast<IAttacker>(MeshComp->GetOwner());
		if (Attacker)
		{
			Attacker->ShouldRotateOff();
		}
	}
}
