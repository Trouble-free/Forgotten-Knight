// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "DeathArmy_AIController.h"
#include "DeathArmy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolPath.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(const FObjectInitializer& ObjectInitializer) :
	UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// attempt to get enemy's controller
	if (ADeathArmy_AIController* const cont = Cast<ADeathArmy_AIController>(OwnerComp.GetAIOwner()))
	{
		// get blackboard component from behavior tree
		if (UBlackboardComponent* const bc = OwnerComp.GetBlackboardComponent())
		{
			// get current patrol path from behavior tree
			const int32 index = bc->GetValueAsInt(GetSelectedBlackboardKey());

			// get enemy
			if (ADeathArmy* enemy = Cast<ADeathArmy>(cont->GetPawn()))
			{
				// get the Patrol Path vector from the Enemy
				APatrolPath* const PatrolPath = enemy->GetPatrolPath();
				if (PatrolPath)
				{
					const FVector Point = PatrolPath->GetPatrolPoint(index);

					// convert the local vector to a global point
					const FVector GlobalPoint = enemy->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
					bc->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

					// finish with success
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
