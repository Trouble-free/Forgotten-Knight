// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "LockOnComponent.h"
#include "BaseSword.h"
#include "BaseShield.h"
#include "ComboSystem.h"
#include "AttackTraceComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Enemy.h"
#include "Boss.h"

void AHeroCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

// Sets default values
AHeroCharacter::AHeroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->TargetOffset = FVector(0, 0, 150);
	CameraBoom->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	LockOnComp = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOnComp"));
	ComboSystem = CreateDefaultSubobject<UComboSystem>(TEXT("Combat System"));
	AttackTraceComp = CreateDefaultSubobject<UAttackTraceComponent>(TEXT("AttackTraceComp"));

	CurrentState = EState::ES_Normal;
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
	StaminaRecoverRate = 30.f;
	StaminaCostRate = 20.f;
	AttackCostRate = 10.f;
	bIsExhausted = false;
	bIsDead = false;
	bBlocking = false;
	ParryDistasnce = 50.f;

	SetupStimulusSource();

}

void AHeroCharacter::SetState(EState State)
{
	CurrentState = State;
}

void AHeroCharacter::OnJump()
{
	if (CurrentState != EState::ES_Normal && CurrentState != EState::ES_Sprint) { return; }
	if (bCanJump)
	{
		Jump();
	}
}

void AHeroCharacter::Move(const FInputActionValue& Value)
{
	if (bStopMoving)
	{
		bStopMoving = false;
		ElapsedTime = 0;
	}
	if (bIsDead) { return; }

	if (bRolling)
	{
		Roll();
		return;
	}

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
#if 1
	float length = MovementVector.Size();
	float WalkSpeed = 230.f;
	float RunSpeed = 500.f;
	float SprintSpeed = 800.f;
	float speed = 0;
	//float ElapsedTime = 0;
	FVector Velocity = GetCharacterMovement()->GetLastUpdateVelocity();
	FVector LateralSpeed = FVector(Velocity.X, Velocity.Y, 0);
	float MovementSpeed = LateralSpeed.Size();
#endif
	if (length < WalkStickMagnitude)
	{	
		if (MovementSpeed > 100.f)
		{
			bStopMoving = true;
			return;
		}
		ElapsedTime = 0;
		speed = 0;
	}
	else if (length < RunStickMagnitude || bIsWalking)
	{
		if (MovementSpeed > WalkSpeed + 100.f)
		{
			bStopMoving = true;
			return;
		}
		if (speed < WalkSpeed)
		{
			ElapsedTime += GetWorld()->GetDeltaSeconds();

		}
		else
		{
			ElapsedTime = 0;
		}
		speed = FMath::Min(WalkSpeed, speed + WalkAcceleration * ElapsedTime);
	}
	else
	{
		float MaxSpeed = RunSpeed;
		if (CurrentState == EState::ES_Sprint)
		{
			MaxSpeed = SprintSpeed;
		}
		if (speed < MaxSpeed)
		{
			ElapsedTime += GetWorld()->GetDeltaSeconds();

		}
		else
		{
			ElapsedTime = 0;
		}
		speed = FMath::Min(MaxSpeed, speed + RunAcceleration * ElapsedTime);
		//speed = bIsWalking ? WalkSpeed : RunSpeed;
	}
	//speed = (CurrentState == EState::ES_Sprint) ? SprintSpeed : speed;

	GetCharacterMovement()->MaxWalkSpeed = speed;

	//UE_LOG(LogTemp, Warning, TEXT("MovementVector: %s, Length: %f, Speed: %f"), *MovementVector.ToString(), length, speed);

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		//AddMovementInput(ForwardDirection, MovementVector.Y);
		//AddMovementInput(RightDirection, MovementVector.X);

		FVector MoveDirection = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
		LastInputDirection = MoveDirection.GetSafeNormal();

		if (CurrentState == EState::ES_Damaged || CurrentState == EState::ES_Parry) { return; }

		if (length >= WalkStickMagnitude)
		{
			MoveDirection.Normalize();
			AddMovementInput(MoveDirection, 1.0f);
		}

		if (GetCharacterMovement()->IsMovingOnGround())
		{
			GetCharacterMovement()->Velocity = MoveDirection * speed;
		}
		
	}
}

void AHeroCharacter::StopMove()
{
	if (!GetCharacterMovement()->IsMovingOnGround()) { return; }

	float speed = GetCharacterMovement()->MaxWalkSpeed;
	float WalkSpeed = 230.f;

	if (speed > WalkSpeed)
	{
		//SlowDownTime += GetWorld()->GetDeltaSeconds();
		speed = FMath::Max(WalkSpeed, speed - SlowDownRate * 2);
	}
	else if (speed > 1.0f)
	{
		speed = FMath::Max(0.0f, speed - SlowDownRate / 2);
	}
	GetCharacterMovement()->MaxWalkSpeed = speed;
	if (speed <= 0.0f)
	{
		ElapsedTime = 0;
		GetCharacterMovement()->StopMovementImmediately();
	}
	else
	{
		AddMovementInput(LastInputDirection, 1.0f);
		GetCharacterMovement()->Velocity = LastInputDirection * speed;
	}
	if (speed < 1.0f)
	{
		ElapsedTime = 0;
		bStopMoving = false;
	}
	
}

void AHeroCharacter::OnStopMove()
{
	bStopMoving = true;
}

void AHeroCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHeroCharacter::LockOn()
{
	if (bIsDead) { return; }
	if (LockOnComp)
	{
		LockOnComp->LockOn();
	}
}

void AHeroCharacter::ToggleWalk(bool bWalkAction)
{
	bIsWalking = bWalkAction;
}

void AHeroCharacter::SwitchTarget(const FInputActionValue& Value)
{
	float InputX = Value.Get<float>();

	LockOnComp->SwitchTarget(InputX);

}

void AHeroCharacter::Attack()
{
	if (bIsDead) { return; }
	bCanJump = false;
	if (Stamina > 1.f)
	{
		AttackCost();
		ComboSystem->Attack();
	}
}

void AHeroCharacter::AttackCost()
{
	CurrentState = EState::ES_Attack;
	if (ComboSystem->bCanAttack)
	{
		float Tmp = Stamina - AttackCostRate;
		if (Tmp > 0)
		{
			Stamina = Tmp;
		}
		else
		{
			Stamina = 0;
		}
	}
}

void AHeroCharacter::HeavyAttack()
{
	if (bIsDead) { return; }
	bCanJump = false;
	if (Stamina > 1.f)
	{
		AttackCost();
		ComboSystem->HeavyAttack();
	}
}

void AHeroCharacter::Sprint()
{
	if (CurrentState != EState::ES_Normal) { return; }

	if (!bIsExhausted) 
	{
		CurrentState = EState::ES_Sprint;
	}
}

void AHeroCharacter::StopSprint()
{
	if (CurrentState == EState::ES_Sprint)
	{
		CurrentState = EState::ES_Normal;
	}
}

void AHeroCharacter::Block()
{
	if (Stamina > 0)
	{
		bBlocking = true;
	}
	else
	{
		bBlocking = false;
	}
}

void AHeroCharacter::StopBlock()
{
	bBlocking = false;
}

void AHeroCharacter::StepBack()
{
	if (GetMovementComponent()->IsFalling()) { return; }

	if (bExecuteAttacking) { return; }

	if (Stamina < 1 || !bCanDodge || bIsExhausted) { return; }
	bCanDodge = false;
	CurrentState = EState::ES_Dodge;
	float Tmp = Stamina - 20.f;
	if (Tmp > 0)
	{
		Stamina = Tmp;
	}
	else
	{
		bIsExhausted = true;
		Stamina = 0;
	}
	PlayAnimMontage(StepBackAnimMontage);
}

void AHeroCharacter::Roll()
{
	bRolling = false;

	if (GetMovementComponent()->IsFalling()) { return; }
	if (Stamina < 1 || !bCanDodge || bIsExhausted) { return; }

	// get rolling direction
	FRotator RollRotation = FRotationMatrix::MakeFromX(LastInputDirection).Rotator();
	SetActorRotation(RollRotation);

	bCanDodge = false;
	CurrentState = EState::ES_Dodge;
	float Tmp = Stamina - 20.f;
	if (Tmp > 0)
	{
		Stamina = Tmp;
	}
	else
	{
		bIsExhausted = true;
		Stamina = 0;
	}
	PlayAnimMontage(RollAnimMontage, 1.5f);
}

void AHeroCharacter::RollTrigger()
{
	if (bExecuteAttacking) { return; }

	if (bCanDodge)
	{
		bRolling = true;
	}
}

void AHeroCharacter::Parry()
{
	if (bIsDead) { return; }
	//bCanJump = false;
	CurrentState = EState::ES_Parry;
	bParrying = true;
	PlayAnimMontage(ParryAnimMontage);

	FHitResult HitResult;

	const FVector ForwardVector = GetActorForwardVector();
	const FVector Start = GetActorLocation() + ForwardVector * ParryDistasnce;
	const FVector End = Start + ForwardVector * ParryDistasnce;

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);

	if (AActor* const HitActor = Cast<AActor>(HitResult.GetActor()))
	{
		const FVector EnemyForwardVector = HitActor->GetActorForwardVector();
		float DotProduct = FVector::DotProduct(ForwardVector, EnemyForwardVector);
		if (DotProduct  < -0.87f)
		{
			if (IEnemy* const Enemy = Cast<IEnemy>(HitActor))
			{
				if (Enemy->CanBeParried())
				{
					Enemy->SetInParriedRange(true);
				}
				//if (CurrentState != EState::ES_Normal)
				//{
				//	CurrentState = EState::ES_Normal;
				//}
			}
		}
	}

}

void AHeroCharacter::ResetState()
{
	CurrentState = EState::ES_Normal;
}

void AHeroCharacter::ResetCanJump()
{
	bCanJump = true;
}

void AHeroCharacter::ResetAttack()
{
	ComboSystem->ResetAttack();
}

void AHeroCharacter::ResetCombo()
{
	ResetCanJump();
	ComboSystem->ResetCombo();
}

void AHeroCharacter::ClearEnemies()
{
	//AttackTraceComp->ActorsToIgnore.Empty();
	for (int32 i = AttackTraceComp->ActorsToIgnore.Num() - 1; i > 0; i--)
	{
		AttackTraceComp->ActorsToIgnore.RemoveAt(i);
	}
}

void AHeroCharacter::RecoveryCanAttack()
{
	ComboSystem->bCanAttack = true;
}

void AHeroCharacter::ResetParrying()
{
	bParrying = false;
}

void AHeroCharacter::ResetDodge()
{
	bCanDodge = true;
}

float AHeroCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IEnemy* const Enemy = Cast<IEnemy>(DamageCauser))
	{
		if (Enemy->IsBeingParried())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Parry Success!"));
			return 0.0f;
		}
	}

	if (GetWorldTimerManager().IsTimerActive(DamagedStateHandle))
	{
		GetWorldTimerManager().ClearTimer(DamagedStateHandle);
	}
	CurrentState = EState::ES_Damaged;

	FVector EnemyForwardVec = DamageCauser->GetActorForwardVector();
	FVector PlayerForwardVec = GetActorForwardVector();
	float DotProduct = FVector::DotProduct(EnemyForwardVec, PlayerForwardVec);

	if (DotProduct < 0 && bBlocking)
	{
		PlayAnimMontage(BlockHitAnimMontage);
		float StaminaTmp = Stamina - DamageAmount * 2;
		DamageAmount = 0.f;
		if (StaminaTmp > 0)
		{
			Stamina = StaminaTmp;
		}
		else
		{
			bCanJump = false;
			Stamina = 0;
			bBlocking = false;
			bCanDodge = false;
			PlayAnimMontage(BlockBreakAnimMontage);
		}
	}
	else
	{
		bCanJump = false;
		bCanDodge = false;
		PlayAnimMontage(HitAnimMontage);
	}

	float Tmp = Health - DamageAmount;
	if (Tmp > 0)
	{
		Health = Tmp;		
		ComboSystem->bCanAttack = false;
	}
	else
	{
		Health = 0;
		bIsDead = true;
		bCanJump = false;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		PlayAnimMontage(DeathAnimMontage);
		if (LockOnComp->TargetActor)
		{
			LockOnComp->DisEngageLock();
		}
		GameOver(); // play game over hud, implement in blueprint
	}

	return 0.0f;
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	FTransform SwordSpawnTransform = GetMesh()->GetSocketTransform(TEXT("sword"));
	ABaseSword* SwordRef = GetWorld()->SpawnActor<ABaseSword>(WeaponType, SwordSpawnTransform);
	if (SwordRef)
	{
		SwordRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("sword"));
	}

	FTransform ShieldSpawnTransform = GetMesh()->GetSocketTransform(TEXT("shield"));
	ABaseShield* ShieldRef = GetWorld()->SpawnActor<ABaseShield>(ShieldType, ShieldSpawnTransform);
	if (ShieldRef)
	{
		ShieldRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("shield"));
	}

	AttackTraceComp->SwordRef = SwordRef;

	if (BossRef)
	{
		BossRef->OnBossDied.AddUObject(this, &AHeroCharacter::DefeatedBoss);
	}
	
}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHeroCharacter::OnJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHeroCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AHeroCharacter::OnStopMove);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHeroCharacter::Look);

		//LockOn
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &AHeroCharacter::LockOn);

		//Walk action
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &AHeroCharacter::ToggleWalk, true);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &AHeroCharacter::ToggleWalk, false);

		//Switch Target
		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Triggered, this, &AHeroCharacter::SwitchTarget);

		//Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AHeroCharacter::Attack);

		//HeavyAttacking
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &AHeroCharacter::HeavyAttack);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHeroCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHeroCharacter::StopSprint);

		//Blocking
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &AHeroCharacter::Block);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AHeroCharacter::StopBlock);

		//Step Back
		EnhancedInputComponent->BindAction(StepBackAction, ETriggerEvent::Triggered, this, &AHeroCharacter::StepBack);

		//Rolling
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AHeroCharacter::RollTrigger);

		//Parrying
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &AHeroCharacter::Parry);
		
	}

}

void AHeroCharacter::Tick(float DeltaTime)
{
	float RecoverRate = StaminaRecoverRate * DeltaTime;
	float CostRate = StaminaCostRate * DeltaTime;
	float Tmp = 0;
	switch (CurrentState)
	{
	case EState::ES_Normal:
		bCanDodge = true;
		ComboSystem->bCanAttack = true;
		bCanJump = true;
		if (Stamina < MaxStamina)
		{
			Tmp = Stamina + RecoverRate;
			Stamina = Tmp;
			if (Stamina > 20.f)
			{
				bIsExhausted = false;
			}
		}
		else
		{
			Stamina = MaxStamina;
		}
		break;
	case EState::ES_Sprint:
		Tmp = Stamina - CostRate;
		if (Tmp > 0)
		{
			Stamina = Tmp;
		}
		else
		{
			Stamina = 0;
			bIsExhausted = true;
			StopSprint();
			SetState(EState::ES_Normal);
		}
		break;
	case EState::ES_Attack:
		break;
	case EState::ES_Damaged:
		break;
	case EState::ES_Dodge:
		break;
	case EState::ES_Parry:
		break;
	default:
		break;
	}

	if (bStopMoving)
	{
		StopMove();
	}

}

void AHeroCharacter::TurnAttackTraceOn()
{
	AttackTraceComp->bIsWeaponActived = true;
}

void AHeroCharacter::TurnAttackTraceOff()
{
	AttackTraceComp->bIsWeaponActived = false;
}

void AHeroCharacter::ShouldRotateOn()
{

}

void AHeroCharacter::ShouldRotateOff()
{

}

bool AHeroCharacter::IsAttacking() const
{
	return false;
}

