// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Boss.h"
#include "Framework/Application/NavigationConfig.h"

void AMainPlayerController::ShowBossHUD()
{
	if (BossHUDAsset)
	{
		BossHUD = CreateWidget<UUserWidget>(this, BossHUDAsset);
	}
	if (BossHUD)
	{
		BossHUD->SetVisibility(ESlateVisibility::Hidden);
		BossHUD->AddToViewport();
	}
}

void AMainPlayerController::SetBossHUDVisible(bool bVisible)
{
	if (BossHUD)
	{
		if (bVisible)
		{
			BossHUD->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			BossHUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}	
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDAsset)
	{
		HUD = CreateWidget<UUserWidget>(this, HUDAsset);
	}
	if (HUD && GetWorld()->GetName() == "Castle")
	{
		HUD->AddToViewport();

		FInputModeGameOnly GameInputMode;
		SetInputMode(GameInputMode);
	}

	ShowBossHUD();

	// Fix UMG bugs in Main Menu Widget Blueprint
	if (IsLocalPlayerController())
	{
		TSharedRef<FNavigationConfig> Navigation = MakeShared<FNavigationConfig>();
		Navigation->bKeyNavigation = false;
		Navigation->bTabNavigation = false;
		Navigation->bAnalogNavigation = false;
		FSlateApplication::Get().SetNavigationConfig(Navigation);
	}

}