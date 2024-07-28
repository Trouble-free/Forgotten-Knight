// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangeRotation.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API UBTService_ChangeRotation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_ChangeRotation();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bIsInBattle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed = 500.f;

	class IAttacker* Attacker;

	UPROPERTY(VisibleAnywhere)
	class ADeathArmy* Enemy;

	UPROPERTY(VisibleAnywhere)
	class ACharacter* Player;

	
};
