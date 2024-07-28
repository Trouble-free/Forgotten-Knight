// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Patrolling.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API UBTTask_Patrolling : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_Patrolling(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FTimerHandle TaskFinishHandle;

	void TaskFinish();

private:
	UBehaviorTreeComponent* CachedOwnerComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol", meta = (AllowPrivateAccess = true))
	float FinishTime;
	
};
