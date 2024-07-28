// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LockOnInterface.h"
#include "HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	TargetAngleDot = 0.0f;

	// ...
}


void ULockOnComponent::TargetDetectTimer()
{
	if (bShouldRotate && TargetActor)
	{
		FRotator NewRotation;

		FVector TargetLoc = TargetActor->GetActorLocation();
		FVector TargetVec = FVector(TargetLoc.X, TargetLoc.Y, TargetLoc.Z - 100.0f);

		NewRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetLoc);

		if (PlayerController)
		{
			PlayerController->SetControlRotation(NewRotation);
		}
	}
}

void ULockOnComponent::BreakLockOn()
{
	if (TargetActor)
	{
		// get distance of player and target
		FVector PlayerLoc = GetOwner()->GetActorLocation();
		FVector TargetLoc = TargetActor->GetActorLocation();
		float Distance = FVector::Distance(PlayerLoc, TargetLoc);
		if (Distance > MaxDistance)
		{
			DisEngageLock();
		}
	}
}

void ULockOnComponent::EnableSwitching()
{
	bCanSwitch = true;
}

// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();


	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerRef = Cast<AHeroCharacter>(GetOwner());
	// ...
	
}

void ULockOnComponent::LockOn()
{
	if (!bIsLockOnEngaged)
	{
		float interval = GetWorld()->GetDeltaSeconds();
		EngageLock();
		GetOwner()->GetWorldTimerManager().SetTimer(TargetDetectTimerHandle, this, &ULockOnComponent::TargetDetectTimer, interval, true);
		GetOwner()->GetWorldTimerManager().SetTimer(BreakLockOnTimerHandle, this, &ULockOnComponent::BreakLockOn, 0.015f, true);
	}
	else
	{
		DisEngageLock();
		GetOwner()->GetWorldTimerManager().ClearTimer(TargetDetectTimerHandle);
		GetOwner()->GetWorldTimerManager().ClearTimer(BreakLockOnTimerHandle);
	}
}

void ULockOnComponent::EngageLock()
{
	FVector start = GetOwner()->GetActorLocation();
	FVector end = start;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;

	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		start,
		end,
		LockOnRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	for (const FHitResult& Hit : HitResults)
	{
		AActor* const HitActor = Hit.GetActor();

		bool bImplementLockOn = UKismetSystemLibrary::DoesImplementInterface(HitActor, ULockOnInterface::StaticClass());
		
		if (bImplementLockOn)
		{
			ILockOnInterface* const LockOnInterface = Cast<ILockOnInterface>(HitActor);
			if (LockOnInterface && !LockOnInterface->IsDead())
			{
				LockOnTargets.AddUnique(HitActor);
				TargetAngleDot = 0.0f;

				for (auto const Target : LockOnTargets)
				{
					FVector ForwardVector = PlayerRef->GetCamera()->GetForwardVector().GetSafeNormal();
					FVector TargetDirection = 
						(Target->GetActorLocation() - PlayerRef->GetActorLocation()).GetSafeNormal();
					float DotProduct = FVector::DotProduct(ForwardVector, TargetDirection);
					if (DotProduct > TargetAngleDot)
					{
						TargetAngleDot = DotProduct;
						TargetActor = Target;
					}
				}
				if (TargetActor)
				{
					bIsLockOnEngaged = true;
					if (PlayerController)
					{
						PlayerController->SetIgnoreLookInput(bIsLockOnEngaged);
					}
					bShouldRotate = true;
					if (!PlayerRef)
					{
						PlayerRef = Cast<AHeroCharacter>(GetOwner());
					}
					PlayerRef->GetCharacterMovement()->bOrientRotationToMovement = false;
					PlayerRef->GetCharacterMovement()->bUseControllerDesiredRotation = true;
				}
			}	
		}
	}
	if (ILockOnInterface* const SelectedTarget = Cast<ILockOnInterface>(TargetActor))
	{
		SelectedTarget->OnSelected();
	}
}

void ULockOnComponent::DisEngageLock()
{
	if (ILockOnInterface* const SelectedTarget = Cast<ILockOnInterface>(TargetActor))
	{
		SelectedTarget->OnDeselected();
	}

	LockOnTargets.RemoveAll([](AActor* Target) {return true; });
	TargetActor = nullptr;
	bShouldRotate = false;
	bIsLockOnEngaged = false;
	if (PlayerController)
	{
		PlayerController->ResetIgnoreLookInput();

	}
	if (!PlayerRef)
	{
		PlayerRef = Cast<AHeroCharacter>(GetOwner());
	}
	PlayerRef->GetCharacterMovement()->bOrientRotationToMovement = true;
	PlayerRef->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void ULockOnComponent::SwitchTarget(const float Value)
{
	if (bIsLockOnEngaged && bCanSwitch)
	{
		if (Value < -0.6f)
		{
			SwitchLeft();
			bCanSwitch = false;
			GetOwner()->GetWorld()->GetTimerManager().SetTimer(SwitchTimerHandle, this, &ULockOnComponent::EnableSwitching, 0.2f, false);
		}
		else if (Value > 0.6f)
		{
			SwitchRight();
			bCanSwitch = false;
			GetOwner()->GetWorld()->GetTimerManager().SetTimer(SwitchTimerHandle, this, &ULockOnComponent::EnableSwitching, 0.2f, false);
		}
	}
}

void ULockOnComponent::SwitchLeft()
{
	if (LockOnTargets.Num() == 0)
	{
		return; // 切り替え目標無し
	}

	StoreTargets.Add(TargetActor);
	LockOnTargets.Remove(TargetActor);
	SwitchAngleDot = 0.0f;
	PotentialTarget = nullptr;

	for (auto const Target : LockOnTargets)
	{
		FVector CurrentLocation = TargetActor->GetActorLocation();
		FVector PlayerLocation = GetOwner()->GetActorLocation();
		FVector PotentialLocation = Target->GetActorLocation();

		FVector CurrentDirection = (CurrentLocation - PlayerLocation).GetSafeNormal();
		FVector PotentialDirection = (PotentialLocation - PlayerLocation).GetSafeNormal();

		float DotProduct = FVector::DotProduct(CurrentDirection, PotentialDirection);
		FVector CrossProduct = FVector::CrossProduct(CurrentDirection, PotentialDirection);


		if (CrossProduct.Z < 0 && DotProduct >= SwitchAngleDot)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("DotProduct: %f"), DotProduct));
			SwitchAngleDot = DotProduct;
			PotentialTarget = Target;
		}
	}
	if (PotentialTarget)
	{
		if (ILockOnInterface* const SelectedTarget = Cast<ILockOnInterface>(TargetActor))
		{
			SelectedTarget->OnDeselected();
		}

		TargetActor = PotentialTarget;
		LockOnTargets.Add(StoreTargets[0]);
		StoreTargets.RemoveAt(0);

		if (ILockOnInterface* const SelectedTarget = Cast<ILockOnInterface>(TargetActor))
		{
			SelectedTarget->OnSelected();
		}
	}
	else
	{
		// 該当の目標が見つからなかったら、TargetActorをStoreTargetsに戻す
		TargetActor = StoreTargets[0];
		LockOnTargets.Add(StoreTargets[0]);
		StoreTargets.RemoveAt(0);
	}
}

void ULockOnComponent::SwitchRight()
{
	if (LockOnTargets.Num() == 0)
	{
		return; // 切り替え目標無し
	}

	StoreTargets.Add(TargetActor);
	LockOnTargets.Remove(TargetActor);
	SwitchAngleDot = 0.0f;
	PotentialTarget = nullptr;

	for (auto const Target : LockOnTargets)
	{
		FVector CurrentLocation = TargetActor->GetActorLocation();
		FVector PlayerLocation = GetOwner()->GetActorLocation();
		FVector PotentialLocation = Target->GetActorLocation();

		FVector CurrentDirection = (CurrentLocation - PlayerLocation).GetSafeNormal();
		FVector PotentialDirection = (PotentialLocation - PlayerLocation).GetSafeNormal();

		float DotProduct = FVector::DotProduct(CurrentDirection, PotentialDirection);
		FVector CrossProduct = FVector::CrossProduct(CurrentDirection, PotentialDirection);

		if (CrossProduct.Z > 0 && DotProduct >= SwitchAngleDot)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("DotProduct: %f"), DotProduct));
			SwitchAngleDot = DotProduct;
			PotentialTarget = Target;
		}
	}
	if (PotentialTarget)
	{
		if (ILockOnInterface* const SelectedTarget = Cast<ILockOnInterface>(TargetActor))
		{
			SelectedTarget->OnDeselected();
		}

		TargetActor = PotentialTarget;
		LockOnTargets.Add(StoreTargets[0]);
		StoreTargets.RemoveAt(0);

		if (ILockOnInterface* const SelectedTarget = Cast<ILockOnInterface>(TargetActor))
		{
			SelectedTarget->OnSelected();
		}
	}
	else
	{
		// 該当の目標が見つからなかったら、TargetActorをStoreTargetsに戻す
		TargetActor = StoreTargets[0];
		LockOnTargets.Add(StoreTargets[0]);
		StoreTargets.RemoveAt(0);
	}
}

