// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterStats.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUILDGAME_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetStats(const FCharacterStats&);

	bool ChangeHealth(int Amount);

	int GetMaxHealth() const
	{
		return Stats.MaxHealth;
	}

	int GetCurrentHealth() const
	{
		return Stats.CurrentHealth;
	}

	void SetMaxHealth(int Amount)
	{
		Stats.MaxHealth = Amount;
	}

	void SetCurrentHealth(int Amount)
	{
		Stats.CurrentHealth = Amount;
		if(Stats.CurrentHealth > Stats.MaxHealth)
		{
			Stats.CurrentHealth = Stats.MaxHealth;
		}
	}
	
	float GetMovementRange() const
	{
		return Stats.MovementRange;
	}

	void SetMovementRange(float NewMovementRange)
	{
		this->Stats.MovementRange = NewMovementRange;
	}

	float GetAttackRange() const
	{
		return Stats.AttackRange;
	}

	void SetAttackRange(float NewAttackRange)
	{
		this->Stats.AttackRange = NewAttackRange;
	}

	int GetBaseDamage() const
	{
		return Stats.BaseDamage;
	}

	void SetBaseDamage(int NewBaseDamage)
	{
		this->Stats.BaseDamage = NewBaseDamage;
	}

	int GetSpeed() const
	{
		return Stats.Speed;
	}

	const TArray<int>& GetSkillIDs() const;
	
private:

	FCharacterStats Stats;
};
