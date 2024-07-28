// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GimmickDoor.generated.h"

UCLASS()
class MELEECOMBAT_API AGimmickDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGimmickDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeLine")
	UCurveFloat* TimelineFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* DoorComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeLine")
	class UTimelineComponent* DoorTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeLine")
	float MoveLength = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeLine")
	float DoorLocationX;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	class ABoss* BossRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTime = 8.0f;

	FTimerHandle ActivateHandle;

private:
	UFUNCTION()
	void OpenDoor(float Value);

	void OnActive();

	void ActivateGimmick();

};
