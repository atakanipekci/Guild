// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsComponent.h"

#include "GGCharacter.h"
#include "GGLogHelper.h"
#include "GuildGame/Managers/CharacterManager.h"
#include "GuildGame/Managers/GridManager.h"

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

bool UCharacterStatsComponent::ChangeHealth(int Amount)
{
	SetCurrentHealth(GetCurrentHealth()+Amount);
	if(GetCurrentHealth() <= 0)
	{
		AGGCharacter* Char = Cast<AGGCharacter> (this->GetOwner());
		if(Char && Char->GetStatus() != ECharacterStatus::Dead)
		{
			Char->OnDeath();
			return true;
		}
	}
	return  false;
}

int UCharacterStatsComponent::ChangeArmor(int Amount)
{
	if(GetCurrentArmor() > 0)
	{
		float Diff = GetCurrentArmor() + Amount;
		SetCurrentArmor(Diff);
		if(Diff <= 0)
		{
			return Diff;
		}
		return  0;
	}
	return Amount;
}

int UCharacterStatsComponent::ChangeMagicArmor(int Amount)
{
	if(GetCurrentMagicArmor() > 0)
	{
		float Diff = GetCurrentMagicArmor() + Amount;
		SetCurrentMagicArmor(Diff);
		if(GetCurrentMagicArmor() <= 0)
		{
			return Diff;
		}
		return  0;
	}

	return Amount;
}

float UCharacterStatsComponent::GetMovementRange() const
{
	GridManager* GridMan =  CharacterManager::CharGridManager;

	int GridSize = 0;
	if(GridMan)
	{
		GridSize = GridMan->GetGridSize();
	}
	return Stats.MovableGridAmountPerAP * GridSize * Stats.CurrentAP;
}

const TArray<int>& UCharacterStatsComponent::GetSkillIDs() const
{
	return Stats.SkillIDs;
}

