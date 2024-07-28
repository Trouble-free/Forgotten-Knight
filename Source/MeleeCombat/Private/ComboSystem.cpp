// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboSystem.h"
#include "HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Boss.h"

// Sets default values for this component's properties
UComboSystem::UComboSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	combo = 0;
	bCanAttack = true;
	DamageRate = 1.0f;
	StabDistasnce = 50.0f;
	StabDamage = 40.f;
	// ...
}


void UComboSystem::Attack()
{
	if (bCanAttack)
	{
		if (CheckBackStab()) 
		{
			PlayerRef->AttackCost();
			return;
		}
		if (CheckExecute())
		{
			PlayerRef->AttackCost();
			return;
		}
		float PlayTime = 0.f;
		switch (combo)
		{
		case 0:
			PlayTime = PlayerRef->PlayAnimMontage(ComboA, 1.2f);
			combo++;
			break;
		case 1:
			PlayTime = PlayerRef->PlayAnimMontage(ComboB, 1.2f);
			combo++;
			break;
		case 2:
			PlayTime = PlayerRef->PlayAnimMontage(ComboA, 1.2f);
			combo++;
			break;
		case 3:
			PlayTime = PlayerRef->PlayAnimMontage(ComboB, 1.2f);
			combo++;
			break;
		case 4:
			PlayTime = PlayerRef->PlayAnimMontage(ComboC, 1.2f);
			combo = 0;
			break;
		}
		bCanAttack = false;
		GetWorld()->GetTimerManager().SetTimer(PlayTimeHandle, this, &UComboSystem::ShouldRegenTimer, PlayTime, false);
	}
}

void UComboSystem::HeavyAttack()
{
	if (bCanAttack)
	{
		DamageRate = 2.0f;
		float PlayTime = 0.f;
		PlayTime = PlayerRef->PlayAnimMontage(HeavyAttackAnimMontage);
		bCanAttack = false;
		GetWorld()->GetTimerManager().SetTimer(PlayTimeHandle, this, &UComboSystem::ShouldRegenTimer, PlayTime, false);
	}

}

bool UComboSystem::CheckBackStab()
{
	FHitResult HitResult;

	const FVector ForwardVector = PlayerRef->GetActorForwardVector();
	const FVector Start = PlayerRef->GetActorLocation() + ForwardVector * StabDistasnce;
	const FVector End = Start + ForwardVector * StabDistasnce;

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);

	if (AActor* const HitActor = Cast<AActor>(HitResult.GetActor()))
	{
		const FVector EnemyForwardVector = HitActor->GetActorForwardVector();
		float DotProduct = FVector::DotProduct(ForwardVector, EnemyForwardVector);
		if (DotProduct > 0.87f)
		{
			if (IEnemy* const Enemy = Cast<IEnemy>(HitActor))
			{
				if (ABoss* const Boss = Cast<ABoss>(Enemy))
				{
					return false; // We can not stab the boss
				}
				float PlayTime = 0.f;
				PlayTime = PlayerRef->PlayAnimMontage(StabAnimMontage);
				PlayerRef->GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
				PlayerRef->GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
				bCanAttack = false;
				PlayerRef->bExecuteAttacking = true;
				GetWorld()->GetTimerManager().SetTimer(ResetCollisionHandle, this, &UComboSystem::ResetCollision, PlayTime, false);
				GetWorld()->GetTimerManager().SetTimer(PlayTimeHandle, this, &UComboSystem::ShouldRegenTimer, PlayTime, false);
				Enemy->Stabbed(StabDamage);
				return true;
			}
		}
	}

	return false;
}

bool UComboSystem::CheckExecute()
{
	FHitResult HitResult;

	const FVector ForwardVector = PlayerRef->GetActorForwardVector();
	const FVector Start = PlayerRef->GetActorLocation() + ForwardVector * StabDistasnce;
	const FVector End = Start + ForwardVector * StabDistasnce;

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);

	if (AActor* const HitActor = Cast<AActor>(HitResult.GetActor()))
	{
		const FVector EnemyForwardVector = HitActor->GetActorForwardVector();
		float DotProduct = FVector::DotProduct(ForwardVector, EnemyForwardVector);
		if (DotProduct < -0.87f)
		{
			if (IEnemy* const Enemy = Cast<IEnemy>(HitActor))
			{
				if (Enemy->IsBeingParried())
				{
					float ExecuteDamage = StabDamage;
					float PlayTime = 0.f;
					if (ABoss* const Boss = Cast<ABoss>(Enemy))
					{
						PlayTime = PlayerRef->PlayAnimMontage(BossExecuteAnimMontage, 0.5f);
						ExecuteDamage = StabDamage * 2;
					}
					else
					{
						PlayTime = PlayerRef->PlayAnimMontage(ExecuteAnimMontage);
					}
					PlayerRef->bExecuteAttacking = true;
					PlayerRef->GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
					PlayerRef->GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
					bCanAttack = false;
					GetWorld()->GetTimerManager().SetTimer(ResetCollisionHandle, this, &UComboSystem::ResetCollision, PlayTime, false);
					GetWorld()->GetTimerManager().SetTimer(PlayTimeHandle, this, &UComboSystem::ShouldRegenTimer, PlayTime, false);
					Enemy->Executed(ExecuteDamage);
					return true;
				}
			}
		}
	}

	return false;
}

// Called when the game starts
void UComboSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...

	PlayerRef = Cast<AHeroCharacter>(GetOwner());
	
}


// Called every frame
void UComboSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UComboSystem::ResetCombo()
{
	combo = 0;
}

void UComboSystem::ResetAttack()
{
	DamageRate = 1.0f;
	bCanAttack = true;
}

void UComboSystem::ShouldRegenTimer()
{
	PlayerRef->bExecuteAttacking = false;
	PlayerRef->CurrentState = EState::ES_Normal;
	GetWorld()->GetTimerManager().ClearTimer(PlayTimeHandle);
}

void UComboSystem::ResetCollision()
{
	PlayerRef->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	PlayerRef->GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetWorld()->GetTimerManager().ClearTimer(ResetCollisionHandle);
}


