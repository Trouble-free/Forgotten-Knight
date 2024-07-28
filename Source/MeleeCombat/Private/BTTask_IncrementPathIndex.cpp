// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"
#include "DeathArmy_AIController.h"
#include "DeathArmy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(const FObjectInitializer& ObjectInitializer) :
	UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// try and get AI controller
	if (ADeathArmy_AIController* const cont = Cast<ADeathArmy_AIController>(OwnerComp.GetAIOwner()))
	{
		// try and get enemy
		if (ADeathArmy* const enemy = Cast<ADeathArmy>(cont->GetPawn()))
		{
			// try and get the blackboard
			if (UBlackboardComponent* const bc = OwnerComp.GetBlackboardComponent())
			{
				// get number of patrol points and min and max indices
				int NoOfPoints = enemy->GetPatrolPath()->Num();
				const int MinIndex = 0;
				const int MaxIndex = NoOfPoints - 1;
				int CurrentIndex = bc->GetValueAsInt(GetSelectedBlackboardKey());

				// change direction when index at first or last if in bidirectional mode
				if (bBiDirectional)
				{
					if (CurrentIndex == MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (CurrentIndex == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				// set new current index to blackboard
				bc->SetValueAsInt(GetSelectedBlackboardKey(),
					(Direction == EDirectionType::Forward ? ++CurrentIndex : --CurrentIndex) % NoOfPoints);

				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
