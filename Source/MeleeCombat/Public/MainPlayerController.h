// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> HUDAsset;

	UUserWidget* HUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> BossHUDAsset;

	UUserWidget* BossHUD;

	void ShowBossHUD();

	void SetBossHUDVisible(bool bVisible);

protected:
	virtual void BeginPlay() override;

	
};
