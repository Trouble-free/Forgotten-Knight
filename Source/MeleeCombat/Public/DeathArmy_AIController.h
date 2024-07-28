// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "DeathArmy_AIController.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API ADeathArmy_AIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit ADeathArmy_AIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(VisibleAnywhere)
	class ADeathArmy* Enemy;

	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	
};
