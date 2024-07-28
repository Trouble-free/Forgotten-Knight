// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AIController.h"
#include "Boss.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void ABoss_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ABoss* const Boss = Cast<ABoss>(InPawn))
	{
		if (UBehaviorTree* const tree = Boss->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}
