// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(BlueprintReadWrite)
	class UTownMenuWidget* MenuWidget;

	UPROPERTY(BlueprintReadWrite)
	class UOverlay* MenuUiContainerOverlay;

	TArray<struct FCharacterStats*> OwnedCharacters;
	
	static void UpdateUI(UObject* Caller);
};
