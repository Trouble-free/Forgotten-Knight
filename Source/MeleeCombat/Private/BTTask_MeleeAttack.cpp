// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"
#include "DeathArmy_AIController.h"
#include "DeathArmy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if out of range do not attack
	const bool bOutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (bOutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// if in range get enemy and its' controller
	ADeathArmy_AIController* const cont = Cast<ADeathArmy_AIController>(OwnerComp.GetAIOwner());
	ADeathArmy* const enemy = Cast<ADeathArmy>(cont->GetPawn());

	// check if the montage has finished
	if (MontageHasFinished(enemy))
	{
		enemy->Attack();
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_MeleeAttack::MontageHasFinished(ADeathArmy* const Enemy)
{
	return Enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Enemy->GetCurrentMontage());
}
