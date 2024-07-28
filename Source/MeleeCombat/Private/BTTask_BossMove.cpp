// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossMove.h"
#include "Boss_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_BossMove::UBTTask_BossMove()
{
	NodeName = TEXT("Moving");
}

EBTNodeResult::Type UBTTask_BossMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	if (ABoss_AIController* const cont = Cast<ABoss_AIController>(OwnerComp.GetAIOwner()))
	{
		const FVector BossLocation = cont->GetPawn()->GetActorLocation();
		const FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->
			GetValueAsVector(GetSelectedBlackboardKey());

		// Calculate the vector from Player to Boss
		FVector PlayerToBoss = (BossLocation - PlayerLocation).GetSafeNormal();

		// Define the movement distance range
		float MinDistance = 300.0f; // Minimum movement distance
		float MaxDistance = 1000.0f; // Maximum movement distance

		// Randomly choose a movement distance within the range
		float MovementDistance = FMath::FRandRange(MinDistance, MaxDistance);

		// Calculate front/back movement point (between Player and Boss)
		FVector FrontBackPoint = FMath::RandRange(80.f, 1000.f) * PlayerToBoss + PlayerLocation;

		// Calculate the right/left movement point on the semicircle
		float Angle = FMath::FRandRange(-90.0f, 90.0f); // Random angle in degrees
		FRotator Rotation(0.0f, Angle, 0.0f);
		FVector RightLeftDirection = Rotation.RotateVector(PlayerToBoss).GetSafeNormal();
		FVector RightLeftPoint = PlayerLocation + RightLeftDirection * MovementDistance;

		// Define possible directions: FrontBack, RightLeft
		TArray<FVector> Directions;
		Directions.Add(FrontBackPoint);
		Directions.Add(RightLeftPoint);

		// Randomly choose a target location
		int32 RandomIndex = FMath::RandRange(0, Directions.Num() - 1);
		FVector TargetLocation = Directions[RandomIndex];

		// Ensure the selected target location is at least MinDistance away from the Boss
		if (FVector::Dist(BossLocation, TargetLocation) < MinDistance)
		{
			//FVector Direction = (TargetLocation - BossLocation).GetSafeNormal();
			//TargetLocation = BossLocation + Direction * MinDistance;
			TargetLocation = PlayerLocation + RightLeftDirection * 500.f;
		}

		// move to the player's location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, TargetLocation);
		GetWorld()->GetTimerManager().SetTimer(
			TaskFinishHandle, this, &UBTTask_BossMove::TaskFinish, FinishTime, false);
		
		return EBTNodeResult::InProgress;

	}

	return EBTNodeResult::Failed;

}

void UBTTask_BossMove::TaskFinish()
{
	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}
