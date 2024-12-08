// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackTraceComponent.h"
#include "BaseSword.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HeroCharacter.h"
#include "DeathArmy.h"
#include "ComboSystem.h"
#include "Boss.h"

// Sets default values for this component's properties
UAttackTraceComponent::UAttackTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ObjectTypesToTrace.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// ...
}


void UAttackTraceComponent::HitStopEnd()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	GetOwner()->GetWorldTimerManager().ClearTimer(DilationTimerHandle);
}

void UAttackTraceComponent::SetHitStop(float duration)
{
	UGameplayStatics::SetGlobalTimeDilation(this, DilationRate);
	GetOwner()->GetWorld()->GetTimerManager().SetTimer(DilationTimerHandle, this,
		&UAttackTraceComponent::HitStopEnd, duration, false);
}

// Called when the game starts
void UAttackTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PlayerRef = Cast<AHeroCharacter>(GetOwner());
	EnemyRef = Cast<ADeathArmy>(GetOwner());
	
	ActorsToIgnore.AddUnique(GetOwner());
	if (EnemyRef)
	{
		for (int32 i = 0; i < EnemyRef->OutActors.Num(); i++)
		{
			Enemies.Push(EnemyRef->OutActors[i]);
		}
		//Enemies.Append(EnemyRef->OutActors);
	}
	if (Enemies.Num() > 0)
	{
		for (AActor* Enemy : Enemies)
		{
			if (Enemy != GetOwner()) // Ensure we don't add the owner to ignore list again
			{
				ActorsToIgnore.Emplace(Enemy);
			}
			//ActorsToIgnore.Append(Enemies);
		}
	}
}


// Called every frame
void UAttackTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bIsWeaponActived)
	{
		FVector Start = SwordRef->GetSwordMesh()->GetSocketLocation(TEXT("start"));
		FVector End = SwordRef->GetSwordMesh()->GetSocketLocation(TEXT("end"));
		TArray<FHitResult> HitResults;
		//ActorsToIgnore.Add(GetOwner());
		UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			Start,
			End,
			SwordRef->TraceSize,
			ObjectTypesToTrace,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResults,
			true);
#if 1
		for (auto OutHit : HitResults)
		{
			AActor* OutActor = OutHit.GetActor();
			if (OutActor && !ActorsToIgnore.Contains(OutActor))
			{
				float Damage = SwordRef->Damage;
				if (PlayerRef)
				{
					Damage *= PlayerRef->GetComboSystem()->DamageRate;
				}

				// Hit Stop
				if (ADeathArmy* const Enemy = Cast<ADeathArmy>(OutActor))
				{
					if (Enemy->Health <= Damage)
					{
						SetHitStop(DilationTime * 10);
						UGameplayStatics::PlaySound2D(this, LastHitSound);
					}
					else
					{
						SetHitStop(DilationTime);
						UGameplayStatics::PlaySound2D(this, ImpactSound);
					}
				}
				else if (ABoss* const Boss = Cast<ABoss>(OutActor))
				{
					if (Boss->Health <= Damage)
					{
						SetHitStop(DilationTime * 20);
						UGameplayStatics::PlaySound2D(this, LastHitSound);
					}
					else
					{
						SetHitStop(DilationTime);
						UGameplayStatics::PlaySound2D(this, ImpactSound);
					}
				}
				else
				{
					SetHitStop(DilationTime);
				}
				
				// Play sound and particle effect
				FRotator ParticleRot = FRotationMatrix::MakeFromZ(OutHit.Normal).Rotator();
				if (AHeroCharacter* const HeroRef = Cast<AHeroCharacter>(OutActor))
				{
					if (HeroRef->bBlocking)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Blocked"));
						UGameplayStatics::PlaySound2D(this, BlockSound);
					}
					else
					{
						//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Hitted"));
						if (HeroRef->Health <= Damage)
						{
							UGameplayStatics::PlaySound2D(this, LastHitSound);
						}
						else
						{
							UGameplayStatics::PlaySound2D(this, ImpactSound);
						}
						
					}
				}
				UGameplayStatics::SpawnEmitterAtLocation(this, BloodParticle, OutHit.Location, ParticleRot);

				UGameplayStatics::ApplyDamage(
					OutActor,
					Damage,
					nullptr,
					GetOwner(),
					DamageTypeClass
				);
				
				ActorsToIgnore.AddUnique(OutActor);
			}
		}
#endif
	}
}

