// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossAttack.h"
#include "Boss_AIController.h"
#include "Boss.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BossAttack::UBTTask_BossAttack()
{
	NodeName = TEXT("Boss Attack");
}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if out of range do not attack
	const bool bOutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (bOutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// if in range get enemy and its' controller
	ABoss_AIController* const cont = Cast<ABoss_AIController>(OwnerComp.GetAIOwner());
	ABoss* const boss = Cast<ABoss>(cont->GetPawn());

	// check if the montage has finished
	if (MontageHasFinished(boss))
	{
		boss->Attack();
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_BossAttack::MontageHasFinished(ABoss* const Boss)
{
	return Boss->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Boss->GetCurrentMontage());
}
