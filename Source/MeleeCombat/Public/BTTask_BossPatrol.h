// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossPatrol.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API UBTTask_BossPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_BossPatrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FTimerHandle TaskFinishHandle;

	UBehaviorTreeComponent* CachedOwnerComp;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol", meta = (AllowPrivateAccess = true))
	float FinishTime;

	void TaskFinish();
	
};
