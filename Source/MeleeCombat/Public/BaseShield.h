// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseShield.generated.h"

UCLASS()
class MELEECOMBAT_API ABaseShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseShield();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* ShieldMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
