// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

void ABossHUD::BeginPlay()
{
    Super::BeginPlay();

    if (BossHealthBarWidgetClass)
    {
        BossHealthBarWidget = CreateWidget<UUserWidget>(GetWorld(), BossHealthBarWidgetClass);
        if (BossHealthBarWidget)
        {
            BossHealthBarWidget->AddToViewport();
        }
    }
}

void ABossHUD::UpdateBossHealthBar(float Health)
{
    if (BossHealthBarWidget)
    {
        UProgressBar* HealthBar = Cast<UProgressBar>(BossHealthBarWidget->GetWidgetFromName(TEXT("HealthProgressBar")));
        if (HealthBar)
        {
            HealthBar->SetPercent(Health);
        }
    }
}
