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
	int Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BaseDamage = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementRange = 850;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Price;
	int UniqueID;
	
	ENpcBehaviourStates TownNpcBehaviourState;

	FCharacterStats()
	{
	}
	explicit FCharacterStats(const FCharacterStats& StructForCopy)
	{
		ClassType = StructForCopy.ClassType;
		ClassName = StructForCopy.ClassName;
		Health = StructForCopy.Health;
		BaseDamage = StructForCopy.BaseDamage;
		MovementRange = StructForCopy.MovementRange;
		AttackRange = StructForCopy.AttackRange;
		Price = StructForCopy.Price;
		TownNpcBehaviourState = StructForCopy.TownNpcBehaviourState;

		if(StructForCopy.UniqueID >= 0)
		{
			UniqueID = StructForCopy.UniqueID;
		}
		else
		{
			
		}
	}

	 bool operator==(const FCharacterStats & InventoryItem) const
     {
         return true;
     }
};