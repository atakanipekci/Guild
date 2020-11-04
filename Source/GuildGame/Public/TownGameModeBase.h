// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "TownDefaultPawn.h"
#include "TownDropBox.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "TownGameModeBase.generated.h"

/**
 * 
 */

USTRUCT()
struct FBuildingDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ConstructionPrice;	
};


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
	}

	 bool operator==(const FCharacterStats & InventoryItem) const
     {
         return true;
     }
};



UCLASS()
class GUILDGAME_API ATownGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	public:
	ATownGameModeBase();

	UPROPERTY(EditAnywhere, Category = Building)
	UDataTable* BuildingDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	UDataTable* CharactersTable;

	class UTownYesOrNoWidget* YesOrNoWidgetInstance;

	UPROPERTY(BlueprintReadWrite)
	class UTownMenuWidget* MenuWidget;

	UPROPERTY(BlueprintReadWrite)
	class UOverlay* MenuUiContainerOverlay;

	TArray<FCharacterStats*> OwnedCharacters;
	
	static void UpdateUI(UObject* Caller);
};
