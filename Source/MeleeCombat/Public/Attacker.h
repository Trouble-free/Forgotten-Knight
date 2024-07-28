// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Attacker.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttacker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MELEECOMBAT_API IAttacker
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TurnAttackTraceOn() = 0;
	virtual void TurnAttackTraceOff() = 0;

	virtual void ShouldRotateOn() = 0;
	virtual void ShouldRotateOff() = 0;

	virtual bool IsAttacking() const = 0;

};
