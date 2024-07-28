// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
URotatingComponent::URotatingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bShouldRotate = false;

	// ...
}


// Called when the game starts
void URotatingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URotatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldRotate)
	{
		// Rotating to target
		AActor* Owner = GetOwner();
		FRotator CurrentRotation = Owner->GetActorRotation();

		if (Target)
		{
			FVector Start = Owner->GetActorLocation();
			FVector TargetLocation = Target->GetActorLocation();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, TargetLocation);
			float Yaw = UKismetMathLibrary::FInterpTo_Constant(CurrentRotation.Yaw, LookAtRotation.Yaw, DeltaTime, 1000);

			Owner->SetActorRotation(FRotator(CurrentRotation.Pitch, Yaw, CurrentRotation.Roll));
		}
	}

	// ...
}

