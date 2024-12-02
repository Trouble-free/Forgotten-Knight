// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LockOnInterface.h"
#include "Attacker.h"
#include "Enemy.h"
#include "Boss.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBossDiedDelegate);

UENUM(BlueprintType)
enum class EBossStatus : uint8
{
	EBoss_Idle UMETA(DisplayName = "Idle"),
	EBoss_Patrol UMETA(DisplayName = "Patrol"),
	EBoss_Attack UMETA(DisplayName = "Attack")
};

UCLASS()
class MELEECOMBAT_API ABoss : public ACharacter, public ILockOnInterface, public IAttacker, public IEnemy
{
	GENERATED_BODY()

	float HealthBarInsideWait = 0;

	class AHeroCharacter* PlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UAttackTraceComponent* AttackTraceComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class URotatingComponent* RotatingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* BossBGM;


public:
	// Sets default values for this character's properties
	ABoss();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EBossStatus CurrentStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	float HealthBarInside;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	float HealthBarInsidePercent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float HealthBarChangeRate;

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
	bool bStaggering;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	int32 AttackCnt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	int32 HitCnt;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	//bool bIsInBattle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EnterAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* HitAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UDataTable* AttackMontageTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StandUpAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ParriedAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ExecutedAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UWidgetComponent* LockOnWidget;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* BGMSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	float FadeOutDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ParriedSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseSword> WeaponType;

	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	virtual bool IsDead() const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Attack();

	void ChangeMode();

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

	virtual void Stabbed(float DamageAmount) override;

	virtual void Executed(float DamageAmount) override;

	virtual void Parried() override;

	FOnBossDiedDelegate OnBossDied;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class USphereComponent* SphereComp;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ClearEnemies();

	UFUNCTION(BlueprintCallable)
	void EndTakeDamage();

	UFUNCTION(BlueprintCallable)
	void EndParried();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void EnterBattle();

	void StandUp();
	void ResetCollision();
	void BossDied();
	void StaggeredEnd();
	void StopPlay();

	FTimerHandle GetUpHandle;
	FTimerHandle ReSetCollisionHandle;
	FTimerHandle ResetAttackModeHandle;
	FTimerHandle OnBossDiedHandle;
	FTimerHandle StaggeredHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UBehaviorTree* GetBehaviorTree() const;

};
