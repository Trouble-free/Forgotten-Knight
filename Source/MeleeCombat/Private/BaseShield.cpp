// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShield.h"

// Sets default values
ABaseShield::ABaseShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(RootComponent);
	ShieldMesh->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void ABaseShield::BeginPlay()
{
	Super::BeginPlay();
	
}

