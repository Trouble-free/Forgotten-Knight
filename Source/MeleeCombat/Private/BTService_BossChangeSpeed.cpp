// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossChangeSpeed.h"
#include "Boss.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_BossChangeSpeed::UBTService_BossChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Boss Change Speed");
}

void UBTService_BossChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (AAIController* const cont = OwnerComp.GetAIOwner())
	{
		if (ABoss* const boss = Cast<ABoss>(cont->GetPawn()))
		{
			boss->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}
