// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackTraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBAT_API UAttackTraceComponent : public UActorComponent
{
	GENERATED_BODY()

	class AHeroCharacter* PlayerRef;

	class ADeathArmy* EnemyRef;

public:	
	// Sets default values for this component's properties
	UAttackTraceComponent();

	UPROPERTY(EditAnywhere)
	class ABaseSword* SwordRef;

	UPROPERTY(EditAnywhere)
	bool bIsWeaponActived;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToTrace;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> Enemies;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	UParticleSystem* BloodParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* BlockSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* LastHitSound;

	//Hit Stop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Stop")
	float DilationRate = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Stop")
	float DilationTime = 0.001f;

	FTimerHandle DilationTimerHandle;

	void HitStopEnd();

	void SetHitStop(float duration);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
