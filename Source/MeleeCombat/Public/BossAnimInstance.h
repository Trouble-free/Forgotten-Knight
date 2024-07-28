// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float ControllerDir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsAttackMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	class ABoss* BossRef;

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperty)
	void UpdateAnimationProperties();
	
};
