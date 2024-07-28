// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enemy.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MELEECOMBAT_API IEnemy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Stabbed(float DamageAmount) = 0;
	virtual void Executed(float DamageAmount) = 0;

	virtual void Parried() = 0;

	virtual void CanBeParriedOn() = 0;
	virtual void CanBeParriedOff() = 0;
	virtual bool IsBeingParried() const = 0;
	virtual bool IsInParriedRange() const = 0;
	virtual void SetInParriedRange(bool bInRange) = 0;
	virtual bool CanBeParried() const = 0;
};
