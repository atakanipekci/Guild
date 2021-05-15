// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsComponent.h"

#include "GGCharacter.h"
#include "GGLogHelper.h"

// Sets default values for this component's properties
UCharacterStatsComponent::UCharacterStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterStatsComponent::SetStats(const FCharacterStats& NewStats)
{
	Stats = NewStats;
}

void UCharacterStatsComponent::ChangeHealth(int Amount)
{
	LOG("Health : %d", GetCurrentHealth());
	SetCurrentHealth(GetCurrentHealth()+Amount);
	LOG("Health : %d", GetCurrentHealth());
	if(GetCurrentHealth() <= 0)
	{
		AGGCharacter* Char = Cast<AGGCharacter> (this->GetOwner());
		if(Char)
		{
			Char->Destroy();
		}
	}
}

