// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	void ChangeHealth(int Amount);

	int GetMaxHealth() const
	{
		return MaxHealth;
	}

	int GetCurrentHealth() const
	{
		return CurrentHealth;
	}

	void SetMaxHealth(int Amount)
	{
		MaxHealth = Amount;
	}

	void SetCurrentHealth(int Amount)
	{
		CurrentHealth = Amount;
	}
	
	float GetMovementRange() const
	{
		return MovementRange;
	}

	void SetMovementRange(float MovementRange)
	{
		this->MovementRange = MovementRange;
	}

	float GetAttackRange() const
	{
		return AttackRange;
	}

	void SetAttackRange(float AttackRange)
	{
		this->AttackRange = AttackRange;
	}

	int GetBaseDamage() const
	{
		return BaseDamage;
	}

	void SetBaseDamage(int BaseDamage)
	{
		this->BaseDamage = BaseDamage;
	}
	
private:
	int MaxHealth;
	int CurrentHealth;
	float MovementRange = 850;
	float AttackRange = 150;
	int BaseDamage = 5;

};
