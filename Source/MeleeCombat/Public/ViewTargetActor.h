// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ViewTargetActor.generated.h"

UCLASS()
class MELEECOMBAT_API AViewTargetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AViewTargetActor();

	void ViewToDoor();

	void ViewToPlayer();

	void ViewHandleEvent();

	void OnBeginBlend();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	class ABoss* BossRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewTarget")
	AActor* ViewTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewTarget")
	float StartTime = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewTarget")
	float BlendTime = 1.0f;

	FTimerHandle BeginBlendTimerHandle;
	FTimerHandle ViewToPlayerHandle;

private:
	APlayerController* PC;

};
