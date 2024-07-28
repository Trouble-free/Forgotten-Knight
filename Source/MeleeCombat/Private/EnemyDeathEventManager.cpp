// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDeathEventManager.h"

// Sets default values
AEnemyDeathEventManager::AEnemyDeathEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyDeathEventManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyDeathEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

