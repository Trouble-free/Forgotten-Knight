// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Attacker.h"
#include "HeroCharacter.generated.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	ES_Normal UMETA(DisplayName = "Normal"),
	ES_Sprint UMETA(DisplayeName = "Sprint"),
	ES_Attack UMETA(DisplayeName = "Attack"),
	ES_Damaged UMETA(DisplayName = "Damaged"),
	ES_Dodge UMETA(DisplayName = "Dodge"),
	ES_Parry UMETA(DisplayName = "Parry")
};

UCLASS()
class MELEECOMBAT_API AHeroCharacter : public ACharacter, public IAttacker
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LockOnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* WalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* StepBackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ParryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float WalkStickMagnitude = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float RunStickMagnitude = 0.98f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class ULockOnComponent* LockOnComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UComboSystem* ComboSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UAttackTraceComponent* AttackTraceComp;

	bool bIsWalking = false;

	bool bCanJump = true;

	bool bCanDodge = true;

	bool bRolling = false;

	FVector LastInputDirection;

	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();

public:
	// Sets default values for this character's properties
	AHeroCharacter();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ABoss* BossRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseSword> WeaponType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseShield> ShieldType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* HitAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BlockHitAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BlockBreakAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StepBackAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* RollAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ParryAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float StaminaRecoverRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float StaminaCostRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float AttackCostRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	bool bIsExhausted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ParryDistasnce;

	bool bParrying = false;

	bool bExecuteAttacking = false;

	void SetState(EState State);

	FTimerHandle DamagedStateHandle;

	void AttackCost(); // Cost stamina when attack

protected:
	// Custom jump function to constriant jump action in some situation 
	void OnJump();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void LockOn();

	void ToggleWalk(bool bWalkAction);

	void SwitchTarget(const FInputActionValue& Value);

	void Attack();

	void HeavyAttack();

	void Sprint();

	void StopSprint();

	void Block();

	void StopBlock();

	void StepBack();

	void Roll();

	void RollTrigger();

	void Parry();

	UFUNCTION(BlueprintCallable)
	void ResetState();

	UFUNCTION(BlueprintCallable)
	void ResetCanJump();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	UFUNCTION(BlueprintCallable)
	void ClearEnemies();

	UFUNCTION(BlueprintCallable)
	void RecoveryCanAttack();

	UFUNCTION(BlueprintCallable)
	void ResetParrying();

	UFUNCTION(BlueprintCallable)
	void ResetDodge();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameOver();

	UFUNCTION(BlueprintImplementableEvent)
	void DefeatedBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	FORCEINLINE class ULockOnComponent* GetLockOnComp() const { return LockOnComp; }

	FORCEINLINE class UComboSystem* GetComboSystem() const { return ComboSystem; }

	FORCEINLINE class UAttackTraceComponent* GetAttackTraceComp() const { return AttackTraceComp; }

	virtual void Tick(float DeltaTime) override;

	virtual void TurnAttackTraceOn() override;
	virtual void TurnAttackTraceOff() override;

	virtual void ShouldRotateOn() override;
	virtual void ShouldRotateOff() override;

	virtual bool IsAttacking() const override;
};
