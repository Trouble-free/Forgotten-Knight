// Fill out your copyright notice in the Description page of Project Settings.


#include "ParriedNotify.h"
#include "Enemy.h"

void UParriedNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		IEnemy* Enemy = Cast<IEnemy>(MeshComp->GetOwner());
		if (Enemy)
		{
			Enemy->CanBeParriedOn();
		}
	}
}

void UParriedNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		IEnemy* Enemy = Cast<IEnemy>(MeshComp->GetOwner());
		if (Enemy)
		{
			Enemy->Parried();
		}
	}
}

void UParriedNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		IEnemy* Enemy = Cast<IEnemy>(MeshComp->GetOwner());
		if (Enemy)
		{
			Enemy->CanBeParriedOff();
		}
	}
}
