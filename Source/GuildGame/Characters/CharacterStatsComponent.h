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

	ECharacterClassType GetCharacterClass() const
	{
		return Stats.ClassType;
	}

	FString GetCharacterClassName() const
	{
		return  Stats.ClassName;
	}

	bool ChangeHealth(int Amount);
	int ChangeArmor(int Amount);
	int ChangeMagicArmor(int Amount);

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
		Stats.CurrentHealth = FMath::Clamp(Stats.CurrentHealth, 0, Stats.MaxHealth);
	}

	int GetMaxArmor() const
	{
		return Stats.MaxArmor;
	}

	int GetCurrentArmor() const
	{
		return Stats.CurrentArmor;
	}

	void SetMaxArmor(int Amount)
	{
		Stats.MaxArmor = Amount;
	}

	void SetCurrentArmor(int Amount)
	{
		Stats.CurrentArmor = Amount;
		Stats.CurrentArmor = FMath::Clamp(Stats.CurrentArmor, 0, Stats.MaxArmor);
	}

	int GetMaxMagicArmor() const
	{
		return Stats.MaxMagicArmor;
	}

	int GetCurrentMagicArmor() const
	{
		return Stats.CurrentMagicArmor;
	}

	void SetMaxMagicArmor(int Amount)
	{
		Stats.MaxMagicArmor = Amount;
	}

	void SetCurrentMagicArmor(int Amount)
	{
		Stats.CurrentMagicArmor = Amount;
		Stats.CurrentMagicArmor = FMath::Clamp(Stats.CurrentMagicArmor, 0, Stats.MaxMagicArmor);
	}

	float GetAccuracy() const
	{
		return Stats.Accuracy;
	}

	void SetAccuracy(float NewAccuracy)
	{
		this->Stats.Accuracy = NewAccuracy;
	}

	float GetDodge() const
	{
		return Stats.Dodge;
	}

	void SetDodge(float NewDodge)
	{
		this->Stats.Dodge = NewDodge;
	}

	float GetLuck() const
	{
		return Stats.Luck;
	}

	void SetLuck(float NewLuck)
	{
		this->Stats.Luck = NewLuck;
	}

	float GetArmorPiercing() const
	{
		return Stats.ArmorPiercing;
	}

	void SetArmorPiercing(float NewArmorPiercing)
	{
		this->Stats.ArmorPiercing = NewArmorPiercing;
	}

	float GetMagicPiercing() const
	{
		return Stats.MagicPiercing;
	}

	void SetMagicPiercing(float NewMagicPiercing)
	{
		this->Stats.MagicPiercing = NewMagicPiercing;
	}

	float GetPhysicalResistance() const
	{
		return Stats.PhysicalResistance;
	}

	void SetPhysicalResistance(float NewPhysicalResistance)
	{
		this->Stats.PhysicalResistance = NewPhysicalResistance;
	}

	float GetMagicalResistance() const
	{
		return Stats.MagicalResistance;
	}

	void SetMagicalResistance(float NewMagicalResistance)
	{
		this->Stats.MagicalResistance = NewMagicalResistance;
	}
	
	float GetMovementRange() const;

	void SetMovementRange(float NewMovementRange)
	{
		this->Stats.MovableGridAmountPerAP = NewMovementRange;
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
	void SetSpeed(int NewSpeed) 
	{
		Stats.Speed = NewSpeed;
	}

	void SetCurrentAP(int NewAP)
	{
		this->Stats.CurrentAP = NewAP;
	}

	int GetCurrentAP() const
	{
		return this->Stats.CurrentAP;
	}

	int GetBaseAP() const
	{
		return this->Stats.BaseAP;
	}

	void SetBaseAP(int NewAP)
	{
		this->Stats.BaseAP = NewAP;
	}

	ECharacterSize GetSize() const
	{
		return Stats.CharacterSize;
	}

	const TArray<int>& GetSkillIDs() const;
	
private:

	FCharacterStats Stats;
};
