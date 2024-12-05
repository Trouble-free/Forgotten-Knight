// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealthPercent(float Percent)
    {
        HealthPercent = Percent;
    }

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetInsidePercent(float Percent)
    {
        InsidePercent = Percent;
    }

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Health")
    float HealthPercent;

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    float InsidePercent;
	
};
