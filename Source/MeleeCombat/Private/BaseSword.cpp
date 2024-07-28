// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSword.h"

// Sets default values
ABaseSword::ABaseSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(RootComponent);
	SwordMesh->SetCollisionProfileName(TEXT("NoCollision"));

	Damage = 10.0f;
	TraceSize = 15.0f;

}

UStaticMeshComponent* ABaseSword::GetSwordMesh() const
{
	return SwordMesh;
}

// Called when the game starts or when spawned
void ABaseSword::BeginPlay()
{
	Super::BeginPlay();
	
}

