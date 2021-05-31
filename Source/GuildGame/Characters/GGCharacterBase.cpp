// Fill out your copyright notice in the Description page of Project Settings.


#include "GGCharacterBase.h"

// Sets default values
AGGCharacterBase::AGGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGGCharacterBase::OnAttackHitsEnemy()
{
}

void AGGCharacterBase::OnDeath()
{
}

void AGGCharacterBase::OnCastingSkillEnds()
{
}

