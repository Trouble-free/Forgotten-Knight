// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBAT_API UComboSystem : public UActorComponent
{
	GENERATED_BODY()

	class AHeroCharacter* PlayerRef;

public:	
	// Sets default values for this component's properties
	UComboSystem();

	UPROPERTY(EditAnywhere)
	int32 combo;

	UPROPERTY(EditAnywhere)
	bool bCanAttack;

	UPROPERTY(EditAnywhere)
	float DamageRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StabDistasnce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StabDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ComboA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ComboB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ComboC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* HeavyAttackAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StabAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ExecuteAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BossExecuteAnimMontage;

	void Attack();

	void HeavyAttack();

	bool CheckBackStab();

	bool CheckExecute();

	void ResetCombo();

	void ResetAttack();

	void ShouldRegenTimer();

	void ResetCollision();

	FTimerHandle PlayTimeHandle;
	FTimerHandle ResetCollisionHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
