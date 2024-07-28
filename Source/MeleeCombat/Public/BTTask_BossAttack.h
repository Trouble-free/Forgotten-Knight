// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossAttack.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API UBTTask_BossAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_BossAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool MontageHasFinished(class ABoss* const Boss);
	
};
