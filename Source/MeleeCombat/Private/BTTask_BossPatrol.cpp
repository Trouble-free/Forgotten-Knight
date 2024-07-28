// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossPatrol.h"
#include "Boss_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_BossPatrol::UBTTask_BossPatrol()
{
	NodeName = TEXT("Boss Is Patrolling");
}

EBTNodeResult::Type UBTTask_BossPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	// get target location from blackboard via the NPC's controller
	if (ABoss_AIController* const cont = Cast<ABoss_AIController>(OwnerComp.GetAIOwner()))
	{
		const FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// move to the player's location
		cont->MoveToLocation(PlayerLocation);
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

		GetWorld()->GetTimerManager().SetTimer(
			TaskFinishHandle, this, &UBTTask_BossPatrol::TaskFinish, FinishTime, false);
		// finish with success
		return EBTNodeResult::InProgress;

	}

	return EBTNodeResult::Failed;
}

void UBTTask_BossPatrol::TaskFinish()
{
	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}

