// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBAT_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

	APlayerController* PlayerController;

	class AHeroCharacter* PlayerRef;

	bool bCanSwitch = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float SwitchStickMagnitude = 0.6f;

public:	
	// Sets default values for this component's properties
	ULockOnComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float LockOnRadius = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MaxDistance = 2300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<AActor*> LockOnTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetAngleDot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	bool bIsLockOnEngaged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	bool bShouldRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<AActor*> StoreTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float SwitchAngleDot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	AActor* PotentialTarget;

	void TargetDetectTimer();
	void BreakLockOn();
	void EnableSwitching();

	FTimerHandle TargetDetectTimerHandle;
	FTimerHandle BreakLockOnTimerHandle;
	FTimerHandle SwitchTimerHandle;

	void LockOn();

	void SwitchTarget(const float Value);

	void EngageLock();

	void DisEngageLock();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SwitchLeft();

	void SwitchRight();

		
};
