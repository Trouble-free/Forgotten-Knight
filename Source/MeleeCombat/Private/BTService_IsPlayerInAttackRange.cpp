// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInAttackRange.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsPlayerInAttackRange::UBTService_IsPlayerInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("In Attack Range?");
}

void UBTService_IsPlayerInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get enemy and its' controller
	AAIController* const cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AActor* const enemy = Cast<AActor>(cont->GetPawn());

	// get player character
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// set true or false to the blackboard key
	bool bIsInRange = enemy->GetDistanceTo(Player) <= AttackRange;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bIsInRange);
}
