// Fill out your copyright notice in the Description page of Project Settings.


#include "HitTrace_NotifyState.h"
#include "Attacker.h"
#include "AttackTraceComponent.h"

void UHitTrace_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		IAttacker* Attacker = Cast<IAttacker>(MeshComp->GetOwner());
		if (Attacker)
		{
			Attacker->TurnAttackTraceOn();
		}
	}
}

void UHitTrace_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		IAttacker* Attacker = Cast<IAttacker>(MeshComp->GetOwner());
		if (Attacker)
		{
			Attacker->TurnAttackTraceOff();
		}
	}
}
