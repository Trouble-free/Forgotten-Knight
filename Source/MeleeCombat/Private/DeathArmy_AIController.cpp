// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathArmy_AIController.h"
#include "DeathArmy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "HeroCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

ADeathArmy_AIController::ADeathArmy_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void ADeathArmy_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ADeathArmy* const npc = Cast<ADeathArmy>(InPawn))
	{
		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}

		Enemy = npc;
	}
}

void ADeathArmy_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 1000.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,
			&ADeathArmy_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void ADeathArmy_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
#if 1
	if (AHeroCharacter* const ch = Cast<AHeroCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
		if (Enemy)
		{
			Enemy->bIsInBattle = Stimulus.WasSuccessfullySensed();
		}
	}
#endif
}
