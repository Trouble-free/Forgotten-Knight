// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeRotation.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Attacker.h"
#include "GameFramework/Character.h"

UBTService_ChangeRotation::UBTService_ChangeRotation()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Rotation");
}

void UBTService_ChangeRotation::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (AAIController* const cont = OwnerComp.GetAIOwner())
	{
		Attacker = Cast<IAttacker>(cont->GetPawn());
		//Enemy = Cast<ADeathArmy>(cont->GetPawn());
		Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
}

void UBTService_ChangeRotation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (Attacker && Player && !Attacker->IsAttacking())
	{
		if (bIsInBattle)
		{
			AActor* const EnemyActor = Cast<AActor>(Attacker);

			if (EnemyActor)
			{
				const FVector Location = EnemyActor->GetActorLocation();
				const FVector PlayerLocation = Player->GetActorLocation();
				const FRotator CurrentRotation = EnemyActor->GetActorRotation();

				FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Location, PlayerLocation);
				float Yaw = UKismetMathLibrary::FInterpTo_Constant(
					CurrentRotation.Yaw, NewRotation.Yaw, DeltaSeconds, RotateSpeed);

				EnemyActor->SetActorRotation(FRotator(CurrentRotation.Pitch, Yaw, CurrentRotation.Roll));
			}
		}
	}
}
