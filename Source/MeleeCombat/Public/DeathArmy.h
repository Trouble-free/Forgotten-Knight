// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LockOnInterface.h"
#include "PatrolPath.h"
#include "Attacker.h"
#include "Enemy.h"
#include "DeathArmy.generated.h"

USTRUCT(BlueprintType)
struct FAttackMontageInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;
};

UCLASS()
class MELEECOMBAT_API ADeathArmy : public ACharacter, public ILockOnInterface, public IAttacker, public IEnemy
{
	GENERATED_BODY()

	class AHeroCharacter* PlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UAttackTraceComponent* AttackTraceComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class URotatingComponent* RotatingComp;

public:
	// Sets default values for this character's properties
	ADeathArmy();

	UPROPERTY(EditAnywhere)
	TArray<AActor*> OutActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	bool bTakingDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	bool bCanBeParried;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	bool bIsBeingParried;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	bool bIsInParriedRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	bool bIsInBattle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* HitAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UDataTable* AttackMontageTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StabbedAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StandUpAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SidleStandUpAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ParriedAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ExecutedAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UWidgetComponent* LockOnWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UWidgetComponent* HealthBarWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ParriedSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseSword> WeaponType;

	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	virtual bool IsDead() const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Attack();

	virtual bool IsAttacking() const override;

	virtual void TurnAttackTraceOn() override;
	virtual void TurnAttackTraceOff() override;

	virtual void ShouldRotateOn() override;
	virtual void ShouldRotateOff() override;

	virtual void CanBeParriedOn() override;
	virtual void CanBeParriedOff() override;
	virtual bool IsBeingParried() const override;
	virtual bool IsInParriedRange() const override;
	virtual void SetInParriedRange(bool bInRange) override;
	virtual bool CanBeParried() const override;

	void HideBar();

	FTimerHandle HealthBarVisionHandle;

	virtual void Stabbed(float DamageAmount) override;

	virtual void Executed(float DamageAmount) override;

	virtual void Parried() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void ClearEnemies();

	UFUNCTION(BlueprintCallable)
	void EndTakeDamage();

	UFUNCTION(BlueprintCallable)
	void EndParried();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHpBar();

	void StandUp();
	void SidleStandUp();
	void ResetCollision();

	FTimerHandle GetUpHandle;
	FTimerHandle ReSetCollisionHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UBehaviorTree* GetBehaviorTree() const;

	APatrolPath* GetPatrolPath() const;

};
