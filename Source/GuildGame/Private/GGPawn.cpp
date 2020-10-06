// Fill out your copyright notice in the Description page of Project Settings.


#include "GGPawn.h"

// Sets default values
AGGPawn::AGGPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AGGPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGGPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

