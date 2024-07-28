// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickDoor.h"
#include "Components/TimelineComponent.h"
#include "Boss.h"

// Sets default values
AGimmickDoor::AGimmickDoor()
{
	DoorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GimmickDoor"));
	DoorComp->SetupAttachment(RootComponent);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

}

// Called when the game starts or when spawned
void AGimmickDoor::BeginPlay()
{
	Super::BeginPlay();

	if (TimelineFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AGimmickDoor::OpenDoor);

		DoorTimeline->AddInterpFloat(TimelineFloat, TimelineProgress);
	}

	if (BossRef)
	{
		BossRef->OnBossDied.AddUObject(this, &AGimmickDoor::ActivateGimmick);
	}
	
	DoorLocationX = GetActorLocation().X;

}

void AGimmickDoor::OpenDoor(float Value)
{
	FVector DoorLocation = GetActorLocation();
	float Distance = MoveLength * Value;

	FVector NewLocation = FVector(DoorLocationX + MoveLength * Value, DoorLocation.Y, DoorLocation.Z);
	SetActorLocation(NewLocation);
}

void AGimmickDoor::OnActive()
{
	GetWorldTimerManager().ClearTimer(ActivateHandle);
	DoorTimeline->Play();
}

void AGimmickDoor::ActivateGimmick()
{
	GetWorldTimerManager().SetTimer(ActivateHandle, this, &AGimmickDoor::OnActive, WaitTime, false);
}

