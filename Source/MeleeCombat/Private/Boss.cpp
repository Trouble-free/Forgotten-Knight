// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
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
#include "DeathArmy.h"
#include "Components/SphereComponent.h"
#include "MainPlayerController.h"
#include "Components/AudioComponent.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentStatus = EBossStatus::EBoss_Idle;

	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidget"));
	LockOnWidget->SetupAttachment(GetCapsuleComponent());
	LockOnWidget->bHiddenInGame = true;

	AttackTraceComp = CreateDefaultSubobject<UAttackTraceComponent>(TEXT("AttackTraceComp"));
	RotatingComp = CreateDefaultSubobject<URotatingComponent>(TEXT("RotatingComp"));

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(2000.f);

	BossBGM = CreateDefaultSubobject<UAudioComponent>(TEXT("BossBGM"));
	BossBGM->bAutoActivate = false;

	GetCharacterMovement()->MaxWalkSpeed = 180.f;

	Health = 300.f;
	MaxHealth = 300.f;
	bIsDead = false;
	bIsAttacking = false;
	bTakingDamage = false;
	bCanBeParried = false;
	bIsBeingParried = false;
	bIsInParriedRange = false;
	bStaggering = false;
	AttackCnt = 0;
	HitCnt = 0;
}

void ABoss::NotifyActorBeginOverlap(AActor* OtherActor)
{
	PlayerRef = Cast<AHeroCharacter>(OtherActor);
	if (PlayerRef)
	{
		if (AMainPlayerController* const PlayerController = Cast<AMainPlayerController>(PlayerRef->GetController()))
		{
			PlayerController->SetBossHUDVisible(true);
		}
		PlayAnimMontage(EnterAnimMontage);
		SphereComp->DestroyComponent(true);
		//ChangeMode();
		if (BossBGM)
		{
			BossBGM->FadeIn(FadeOutDuration, 1.0f);
			//BossBGM->Play();
		}

		CurrentStatus = EBossStatus::EBoss_Patrol;
		AController* EnemyController = GetController();
		AAIController* EnemyAIController;
		if (EnemyController)
		{
			EnemyAIController = Cast<AAIController>(EnemyController);
			if (EnemyAIController)
			{
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool("SeePlayer", true);
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool("FinishAttackCount", true);
			}
		}	
	}
	
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
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

	if (BGMSound)
	{
		BossBGM->SetSound(BGMSound);
	}

	OnBossDied.AddUObject(this, &ABoss::StopPlay);
	
}

void ABoss::ClearEnemies()
{
	for (int32 i = AttackTraceComp->ActorsToIgnore.Num() - 1; i > 0; i--)
	{
		AttackTraceComp->ActorsToIgnore.RemoveAt(i);
	}
}

void ABoss::EndTakeDamage()
{
	bIsBeingParried = false;
	bTakingDamage = false;
}

void ABoss::EndParried()
{
	bIsBeingParried = false;
}

void ABoss::AttackEnd()
{
	bIsAttacking = false;
}

void ABoss::EnterBattle()
{
	CurrentStatus = EBossStatus::EBoss_Attack;

	AController* EnemyController = GetController();
	AAIController* EnemyAIController;
	if (EnemyController)
	{
		EnemyAIController = Cast<AAIController>(EnemyController);
		if (EnemyAIController)
		{
			//EnemyAIController->GetBlackboardComponent()->SetValueAsBool("SeePlayer", true);
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("FinishAttackCount", false);
		}
	}
}

void ABoss::StandUp()
{
	GetWorldTimerManager().ClearTimer(GetUpHandle);
	float PlayTime = PlayAnimMontage(StandUpAnimMontage);
	GetWorldTimerManager().SetTimer(ReSetCollisionHandle, this, &ABoss::ResetCollision, PlayTime, false);
}

void ABoss::ResetCollision()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetWorldTimerManager().ClearTimer(ReSetCollisionHandle);
}

void ABoss::BossDied()
{
	if (PlayerRef)
	{
		if (AMainPlayerController* const PlayerController = Cast<AMainPlayerController>(PlayerRef->GetController()))
		{
			PlayerController->SetBossHUDVisible(false);
		}
	}
	GetWorldTimerManager().ClearTimer(OnBossDiedHandle);
}

void ABoss::StaggeredEnd()
{
	bStaggering = false;
	GetWorldTimerManager().ClearTimer(StaggeredHandle);
}

void ABoss::StopPlay()
{
	if (BossBGM)
	{
		BossBGM->FadeOut(FadeOutDuration, 0.0f);
		//BossBGM->Stop();
	}
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBehaviorTree* ABoss::GetBehaviorTree() const
{
	return Tree;
}

void ABoss::OnSelected()
{
	LockOnWidget->bHiddenInGame = false;
}

void ABoss::OnDeselected()
{
	LockOnWidget->bHiddenInGame = true;
}

bool ABoss::IsDead() const
{
	return bIsDead;
}

float ABoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Tmp = Health - DamageAmount;
	if (Tmp > 0)
	{
		bTakingDamage = true;
		Health = Tmp;
		AttackCnt = 0;
		AController* EnemyController = GetController();
		AAIController* EnemyAIController;
		if (EnemyController)
		{
			EnemyAIController = Cast<AAIController>(EnemyController);
			if (EnemyAIController)
			{
				if (CurrentStatus == EBossStatus::EBoss_Patrol)
				{
					CurrentStatus = EBossStatus::EBoss_Attack;
					EnemyAIController->GetBlackboardComponent()->SetValueAsBool("FinishAttackCount", false);
				}
			}
		}
		if (!bStaggering)
		{
			HitCnt++;
		}
		if (HitCnt > 9 && !bStaggering)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("IS STAGGERING"));
			bStaggering = true;
			HitCnt = 0;
			float PlayTime = PlayAnimMontage(HitAnimMontage);
			GetCharacterMovement()->StopMovementImmediately();
			GetWorldTimerManager().SetTimer(StaggeredHandle, this, &ABoss::StaggeredEnd, PlayTime + 1.f);
			if (EnemyController)
			{
				EnemyAIController = Cast<AAIController>(EnemyController);
				if (EnemyAIController)
				{
					EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsDamaged", true);
				}
			}
		}

		// Update HealthBar
		float HealthPercent = Health / MaxHealth;

	}
	else
	{
		Health = 0;
		bIsDead = true;
		OnBossDied.Broadcast();
		GetWorldTimerManager().SetTimer(OnBossDiedHandle, this, &ABoss::BossDied, 5.f, false);
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
				//PlayerRef->GetLockOnComp()->EngageLock();
			}
		}
	}

	return 0.0f;
}

void ABoss::Attack()
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
				AttackCnt++;
				if (AttackCnt > 2)
				{
					ChangeMode();
					AttackCnt = 0;
				}
	
			}
		}
	}
}

void ABoss::ChangeMode()
{
	if (GetWorldTimerManager().IsTimerActive(ResetAttackModeHandle))
	{
		GetWorldTimerManager().ClearTimer(ResetAttackModeHandle);
	}
	AController* EnemyController = GetController();
	AAIController* EnemyAIController;
	if (EnemyController)
	{
		EnemyAIController = Cast<AAIController>(EnemyController);
		if (EnemyAIController)
		{
			if (CurrentStatus == EBossStatus::EBoss_Patrol)
			{
				CurrentStatus = EBossStatus::EBoss_Attack;
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool("FinishAttackCount", false);
			}
			else if (CurrentStatus == EBossStatus::EBoss_Attack)
			{
				CurrentStatus = EBossStatus::EBoss_Patrol;
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool("FinishAttackCount", true);
				GetWorldTimerManager().SetTimer(ResetAttackModeHandle, this, &ABoss::ChangeMode, 20.f, false);
			}
		}
	}
}

bool ABoss::IsAttacking() const
{
	return bIsAttacking;
}

void ABoss::TurnAttackTraceOn()
{
	AttackTraceComp->bIsWeaponActived = true;
}

void ABoss::TurnAttackTraceOff()
{
	AttackTraceComp->bIsWeaponActived = false;
}

void ABoss::ShouldRotateOn()
{
	RotatingComp->bShouldRotate = true;
}

void ABoss::ShouldRotateOff()
{
	RotatingComp->bShouldRotate = false;
}

void ABoss::CanBeParriedOn()
{
	bCanBeParried = true;
}

void ABoss::CanBeParriedOff()
{
	bCanBeParried = false;
}

bool ABoss::IsBeingParried() const
{
	return bIsBeingParried;
}

bool ABoss::IsInParriedRange() const
{
	return bIsInParriedRange;
}

void ABoss::SetInParriedRange(bool bInRange)
{
	bIsInParriedRange = bInRange;
}

bool ABoss::CanBeParried() const
{
	return bCanBeParried;
}

void ABoss::Stabbed(float DamageAmount)
{
}

void ABoss::Executed(float DamageAmount)
{
	float Tmp = Health - DamageAmount;
	if (Tmp > 0)
	{
		bTakingDamage = true;
		Health = Tmp;

		// Update HealthBar
		float HealthPercent = Health / MaxHealth;

		float PlayTime = PlayAnimMontage(ExecutedAnimMontage, 0.5f);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GetWorldTimerManager().SetTimer(GetUpHandle, this, &ABoss::StandUp, PlayTime, false);
		GetCharacterMovement()->StopMovementImmediately();
	}
	else
	{
		Health = 0;
		bIsDead = true;
		OnBossDied.Broadcast();
		GetWorldTimerManager().SetTimer(OnBossDiedHandle, this, &ABoss::BossDied, 5.f, false);
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
				//PlayerRef->GetLockOnComp()->EngageLock();
			}
		}
	}
}

void ABoss::Parried()
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
