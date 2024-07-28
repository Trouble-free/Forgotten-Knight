// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BossHUD.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API ABossHUD : public AHUD
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    void UpdateBossHealthBar(float Health);

private:
    UPROPERTY(EditAnywhere, Category = "Widgets")
    TSubclassOf<class UUserWidget> BossHealthBarWidgetClass;

    UPROPERTY()
    UUserWidget* BossHealthBarWidget;
	
};
