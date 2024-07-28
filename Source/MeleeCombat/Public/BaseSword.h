// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSword.generated.h"

UENUM(BlueprintType)
enum class EMeshType : uint8
{
	MT_STATIC  UMETA(DisplayName = "Static"),
	MT_SKELETAL UMETA(DisplayName = "Skeleton")
};

UCLASS()
class MELEECOMBAT_API ABaseSword : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* SwordMesh;
	
public:	
	// Sets default values for this actor's properties
	ABaseSword();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float TraceSize;

	UStaticMeshComponent* GetSwordMesh() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
