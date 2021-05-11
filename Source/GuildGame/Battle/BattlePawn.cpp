// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePawn.h"

// Sets default values
ABattlePawn::ABattlePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ABattlePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

