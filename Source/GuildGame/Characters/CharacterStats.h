#pragma once
#include "Engine/DataTable.h"
#include "CharacterStats.generated.h"

enum class ENpcBehaviourStates: uint8;

UENUM()
enum class ECharacterClassType: uint8
{
	Knight,
	Mage
};

USTRUCT()
struct FCharacterStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterClassType ClassType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BaseDamage = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementRange = 850;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> SkillIDs;

	int SpendableSkillPoints = 0;
	
	int UniqueID;
	
	ENpcBehaviourStates TownNpcBehaviourState;

	FCharacterStats() = default;
	
	explicit FCharacterStats(const FCharacterStats& StructForCopy)
	{
		ClassType = StructForCopy.ClassType;
		ClassName = StructForCopy.ClassName;
		CurrentHealth = StructForCopy.CurrentHealth;
		MaxHealth = StructForCopy.MaxHealth;
		BaseDamage = StructForCopy.BaseDamage;
		MovementRange = StructForCopy.MovementRange;
		AttackRange = StructForCopy.AttackRange;
		Price = StructForCopy.Price;
		TownNpcBehaviourState = StructForCopy.TownNpcBehaviourState;
		SkillIDs = StructForCopy.SkillIDs;
		SpendableSkillPoints = StructForCopy.SpendableSkillPoints;
		
		if(StructForCopy.UniqueID >= 0)
		{
			UniqueID = StructForCopy.UniqueID;
		}
		else
		{
			
		}
	}

	 bool operator==(const FCharacterStats & OtherStats) const
     {
         return true;
     }
};