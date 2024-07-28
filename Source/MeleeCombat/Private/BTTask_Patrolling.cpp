// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrolling.h"
#include "DeathArmy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_Patrolling::UBTTask_Patrolling(const FObjectInitializer& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Patrolling");
}

EBTNodeResult::Type UBTTask_Patrolling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	// get target location from blackboard via the NPC's controller
	if (ADeathArmy_AIController* const cont = Cast<ADeathArmy_AIController>(OwnerComp.GetAIOwner()))
	{
		const FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		//FinishTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("MoveTime");

		// move to the player's location
		cont->MoveToLocation(PlayerLocation);
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

		GetWorld()->GetTimerManager().SetTimer(
			TaskFinishHandle, this, &UBTTask_Patrolling::TaskFinish, FinishTime, false);
		// finish with success
		return EBTNodeResult::InProgress;

	}

	return EBTNodeResult::Failed;
}

void UBTTask_Patrolling::TaskFinish()
{
	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}
