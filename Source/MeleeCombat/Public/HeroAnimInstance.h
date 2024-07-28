// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsLockOn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float ControllerDir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	class AHeroCharacter* PlayerRef;

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category=AnimationProperty)
	void UpdateAnimationProperties();
	
};
