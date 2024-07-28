// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewTargetActor.h"
#include "Boss.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AViewTargetActor::AViewTargetActor()
{

}

void AViewTargetActor::ViewToDoor()
{
	if (GetWorldTimerManager().IsTimerActive(BeginBlendTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(BeginBlendTimerHandle);
	}
	if (ViewTarget && PC)
	{
		PC->SetViewTargetWithBlend(ViewTarget, BlendTime);
		GetWorldTimerManager().SetTimer(ViewToPlayerHandle, this, &AViewTargetActor::ViewHandleEvent, 3.f, false);
	}
}

void AViewTargetActor::ViewToPlayer()
{
	if (GetWorldTimerManager().IsTimerActive(ViewToPlayerHandle))
	{
		GetWorldTimerManager().ClearTimer(ViewToPlayerHandle);
	}
	if (PC)
	{
		//PC->SetViewTargetWithBlend(PC->GetPawn(), BlendTime);
		PC->SetViewTarget(PC->GetPawn());
	}
}

void AViewTargetActor::ViewHandleEvent()
{
	ViewToPlayer();
}

void AViewTargetActor::OnBeginBlend()
{
	GetWorldTimerManager().SetTimer(BeginBlendTimerHandle, this, &AViewTargetActor::ViewToDoor, StartTime, false);
}

// Called when the game starts or when spawned
void AViewTargetActor::BeginPlay()
{
	Super::BeginPlay();

	if (BossRef)
	{
		BossRef->OnBossDied.AddUObject(this, &AViewTargetActor::OnBeginBlend);
	}

	PC = UGameplayStatics::GetPlayerController(this, 0);
	
}

