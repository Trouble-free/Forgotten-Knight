// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathArmy.h"
#include "DeathArmy_AIController.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HeroCharacter.h"
#include "LockOnComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseSword.h"
#include "AttackTraceComponent.h"
#include "RotatingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HealthBarWidget.h"

// Sets default values
ADeathArmy::ADeathArmy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidget"));
	LockOnWidget->SetupAttachment(GetCapsuleComponent());
	LockOnWidget->bHiddenInGame = true;

	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Widget"));
	HealthBarWidgetComp->SetupAttachment(GetCapsuleComponent());
	HealthBarWidgetComp->bHiddenInGame = true;

	AttackTraceComp = CreateDefaultSubobject<UAttackTraceComponent>(TEXT("AttackTraceComp"));
	RotatingComp = CreateDefaultSubobject<URotatingComponent>(TEXT("RotatingComp"));

	GetCharacterMovement()->MaxWalkSpeed = 100.f;

	Health = 50.f;
	MaxHealth = 50.f;
	bIsDead = false;
	bIsAttacking = false;
	bTakingDamage = false;
	bCanBeParried = false;
	bIsBeingParried = false;
	bIsInParriedRange = false;
	bIsInBattle = false;
	Tags.Add(FName("enemy"));

}

void ADeathArmy::OnSelected()
{
	LockOnWidget->bHiddenInGame = false;
}

void ADeathArmy::OnDeselected()
{
	LockOnWidget->bHiddenInGame = true;
}

bool ADeathArmy::IsDead() const
{
	return bIsDead;
}

float ADeathArmy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Tmp = Health - DamageAmount;
	if (Tmp > 0)
	{
		if (GetWorldTimerManager().IsTimerActive(HealthBarVisionHandle))
		{
			GetWorldTimerManager().ClearTimer(HealthBarVisionHandle);
		}
		
		GetWorldTimerManager().SetTimer(HealthBarVisionHandle, this, &ADeathArmy::HideBar, 5.f, false);
		bTakingDamage = true;
		Health = Tmp;

		// Update HealthBar
		float HealthPercent = Health / MaxHealth;
		HealthBarWidgetComp->bHiddenInGame = false;
		UHealthBarWidget* HealthBar = Cast<UHealthBarWidget>(HealthBarWidgetComp->GetUserWidgetObject());
		if (HealthBar)
		{
			HealthBar->SetHealthPercent(HealthPercent);
		}

		UpdateHpBar();
		PlayAnimMontage(HitAnimMontage);
		GetCharacterMovement()->StopMovementImmediately();
		AController* EnemyController = GetController();
		AAIController* EnemyAIController;
		if (EnemyController)
		{
			EnemyAIController = Cast<AAIController>(EnemyController);
			if (EnemyAIController)
			{
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsDamaged", true);
			}
		}
	}
	else
	{
		HealthBarWidgetComp->bHiddenInGame = true;
		Health = 0;
		bIsDead = true;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		PlayAnimMontage(DeathAnimMontage);
		AController* EnemyController = GetController();
		AAIController* EnemyAIController;
		if (EnemyController)
		{
			EnemyAIController = Cast<AAIController>(EnemyController);
			if (EnemyAIController)
			{
				EnemyAIController->BrainComponent->StopLogic(TEXT("Died"));
			}
		}

		if (PlayerRef)
		{
			if (this == PlayerRef->GetLockOnComp()->TargetActor)
			{
				PlayerRef->GetLockOnComp()->DisEngageLock();
				PlayerRef->GetLockOnComp()->EngageLock();
			}
		}
	}

	return 0.0f;
}

void ADeathArmy::Attack()
{
	if (AttackMontageTable)
	{
		TArray<FName> RowNames = AttackMontageTable->GetRowNames();

		if (RowNames.Num() > 0)
		{
			FName RandomRowName = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];

			FAttackMontageInfo* Row = AttackMontageTable->FindRow<FAttackMontageInfo>(RandomRowName, TEXT("AttackMontage"));

			if (Row && Row->AttackMontage)
			{
				PlayAnimMontage(Row->AttackMontage);
				bIsAttacking = true;
			}
		}
	}
}

bool ADeathArmy::IsAttacking() const
{
	return bIsAttacking;
}

void ADeathArmy::TurnAttackTraceOn()
{
	AttackTraceComp->bIsWeaponActived = true;
}

void ADeathArmy::TurnAttackTraceOff()
{
	AttackTraceComp->bIsWeaponActived = false;
}

void ADeathArmy::ShouldRotateOn()
{
	RotatingComp->bShouldRotate = true;
}

void ADeathArmy::ShouldRotateOff()
{
	RotatingComp->bShouldRotate = false;
}

void ADeathArmy::CanBeParriedOn()
{
	bCanBeParried = true;
}

void ADeathArmy::CanBeParriedOff()
{
	bCanBeParried = false;
}

bool ADeathArmy::IsBeingParried() const
{
	return bIsBeingParried;
}

bool ADeathArmy::IsInParriedRange() const
{
	return bIsInParriedRange;
}

void ADeathArmy::SetInParriedRange(bool bInRange)
{
	bIsInParriedRange = bInRange;
}

bool ADeathArmy::CanBeParried() const
{
	return bCanBeParried;
}

void ADeathArmy::HideBar()
{
	HealthBarWidgetComp->bHiddenInGame = true;
	GetWorldTimerManager().ClearTimer(HealthBarVisionHandle);
}

void ADeathArmy::Stabbed(float DamageAmount)
{
	float Tmp = Health - DamageAmount;
	if (Tmp > 0)
	{
		if (GetWorldTimerManager().IsTimerActive(HealthBarVisionHandle))
		{
			GetWorldTimerManager().ClearTimer(HealthBarVisionHandle);
		}

		GetWorldTimerManager().SetTimer(HealthBarVisionHandle, this, &ADeathArmy::HideBar, 5.f, false);
		bTakingDamage = true;
		Health = Tmp;

		// Update HealthBar
		float HealthPercent = Health / MaxHealth;
		HealthBarWidgetComp->bHiddenInGame = false;
		UHealthBarWidget* HealthBar = Cast<UHealthBarWidget>(HealthBarWidgetComp->GetUserWidgetObject());
		if (HealthBar)
		{
			HealthBar->SetHealthPercent(HealthPercent);
		}

		UpdateHpBar();
		float PlayTime = PlayAnimMontage(StabbedAnimMontage);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GetWorldTimerManager().SetTimer(GetUpHandle, this, &ADeathArmy::StandUp, PlayTime, false);
		GetCharacterMovement()->StopMovementImmediately();
		AController* EnemyController = GetController();
		AAIController* EnemyAIController;
		if (EnemyController)
		{
			EnemyAIController = Cast<AAIController>(EnemyController);
			if (EnemyAIController)
			{
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsDamaged", true);
			}
		}
	}
	else
	{
		HealthBarWidgetComp->bHiddenInGame = true;
		Health = 0;
		bIsDead = true;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		PlayAnimMontage(StabbedAnimMontage);
		AController* EnemyController = GetController();
		AAIController* EnemyAIController;
		if (EnemyController)
		{
			EnemyAIController = Cast<AAIController>(EnemyController);
			if (EnemyAIController)
			{
				EnemyAIController->BrainComponent->StopLogic(TEXT("Died"));
			}
		}

		if (PlayerRef)
		{
			if (this == PlayerRef->GetLockOnComp()->TargetActor)
			{
				PlayerRef->GetLockOnComp()->DisEngageLock();
				PlayerRef->GetLockOnComp()->EngageLock();
			}
		}
	}
}

void ADeathArmy::Executed(float DamageAmount)
{
	float Tmp = Health - DamageAmount;
	if (Tmp > 0)
	{
		if (GetWorldTimerManager().IsTimerActive(HealthBarVisionHandle))
		{
			GetWorldTimerManager().ClearTimer(HealthBarVisionHandle);
		}

		GetWorldTimerManager().SetTimer(HealthBarVisionHandle, this, &ADeathArmy::HideBar, 5.f, false);
		bTakingDamage = true;
		Health = Tmp;

		// Update HealthBar
		float HealthPercent = Health / MaxHealth;
		HealthBarWidgetComp->bHiddenInGame = false;
		UHealthBarWidget* HealthBar = Cast<UHealthBarWidget>(HealthBarWidgetComp->GetUserWidgetObject());
		if (HealthBar)
		{
			HealthBar->SetHealthPercent(HealthPercent);
		}

		UpdateHpBar();
		float PlayTime = PlayAnimMontage(ExecutedAnimMontage);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GetWorldTimerManager().SetTimer(GetUpHandle, this, &ADeathArmy::SidleStandUp, PlayTime, false);
		GetCharacterMovement()->StopMovementImmediately();
		AController* EnemyController = GetController();
		AAIController* EnemyAIController;
		if (EnemyController)
		{
			EnemyAIController = Cast<AAIController>(EnemyController);
			if (EnemyAIController)
			{
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsDamaged", true);
			}
		}
	}
	else
	{
		HealthBarWidgetComp->bHiddenInGame = true;
		Health = 0;
		bIsDead = true;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		PlayAnimMontage(ExecutedAnimMontage);
		AController* EnemyController = GetController();
		AAIController* EnemyAIController;
		if (EnemyController)
		{
			EnemyAIController = Cast<AAIController>(EnemyController);
			if (EnemyAIController)
			{
				EnemyAIController->BrainComponent->StopLogic(TEXT("Died"));
			}
		}

		if (PlayerRef)
		{
			if (this == PlayerRef->GetLockOnComp()->TargetActor)
			{
				PlayerRef->GetLockOnComp()->DisEngageLock();
				PlayerRef->GetLockOnComp()->EngageLock();
			}
		}
	}
}

void ADeathArmy::Parried()
{
	if (PlayerRef->bParrying && bCanBeParried && bIsInParriedRange)
	{
		UGameplayStatics::PlaySound2D(this, ParriedSound);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Being Parried"));
		PlayAnimMontage(ParriedAnimMontage, 0.5f);
		bIsBeingParried = true;
		bCanBeParried = false;
		bIsInParriedRange = false;
	}
}

// Called when the game starts or when spawned
void ADeathArmy::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PlayerRef = Cast<AHeroCharacter>(Player);

	FTransform SwordSpawnTransform = GetMesh()->GetSocketTransform(TEXT("sword"));
	ABaseSword* SwordRef = GetWorld()->SpawnActor<ABaseSword>(WeaponType, SwordSpawnTransform);
	if (SwordRef)
	{
		SwordRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("sword"));
	}

	AttackTraceComp->SwordRef = SwordRef;
	RotatingComp->Target = PlayerRef;

	// Initial Health Bar
	float HealthPercent = Health / MaxHealth;
	UHealthBarWidget* HealthBar = Cast<UHealthBarWidget>(HealthBarWidgetComp->GetUserWidgetObject());
	if (HealthBar)
	{
		HealthBar->SetHealthPercent(HealthPercent);
	}

#if 0
	if (OutActors.Num() > 0)
	{
		AttackTraceComp->Enemies.Append(OutActors);
		AttackTraceComp->ActorsToIgnore.Append(AttackTraceComp->Enemies);
	}
#endif
	
}

void ADeathArmy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// get all tags of enemy to set attack trace enemies array. DONT PUSH THIS INTO BeginPlay()!!!
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("enemy"), OutActors);

}

void ADeathArmy::ClearEnemies()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::FromInt(IgnoreCount));
	//AttackTraceComp->ActorsToIgnore.RemoveAt(AttackTraceComp->ActorsToIgnore.Num() - 1);
	AttackTraceComp->ActorsToIgnore.RemoveAll([](AActor* Actor) {
		return Actor = Cast<AHeroCharacter>(Actor);
		});
}

void ADeathArmy::EndTakeDamage()
{
	bIsBeingParried = false;
	bTakingDamage = false;
	AController* EnemyController = GetController();
	AAIController* EnemyAIController;
	if (EnemyController)
	{
		EnemyAIController = Cast<AAIController>(EnemyController);
		if (EnemyAIController)
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsDamaged", false);
		}
	}

}

void ADeathArmy::EndParried()
{
	bIsBeingParried = false;
}

void ADeathArmy::AttackEnd()
{
	bIsAttacking = false;
}

void ADeathArmy::StandUp()
{
	GetWorldTimerManager().ClearTimer(GetUpHandle);
	float PlayTime = PlayAnimMontage(StandUpAnimMontage);
	GetWorldTimerManager().SetTimer(ReSetCollisionHandle, this, &ADeathArmy::ResetCollision, PlayTime, false);
}

void ADeathArmy::SidleStandUp()
{
	GetWorldTimerManager().ClearTimer(GetUpHandle);
	float PlayTime = PlayAnimMontage(SidleStandUpAnimMontage);
	GetWorldTimerManager().SetTimer(ReSetCollisionHandle, this, &ADeathArmy::ResetCollision, PlayTime, false);
}

void ADeathArmy::ResetCollision()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetWorldTimerManager().ClearTimer(ReSetCollisionHandle);
}

// Called every frame
void ADeathArmy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBehaviorTree* ADeathArmy::GetBehaviorTree() const
{
	return Tree;
}

APatrolPath* ADeathArmy::GetPatrolPath() const
{
	return PatrolPath;
}

