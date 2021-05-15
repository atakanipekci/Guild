// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "TownGameModeBase.generated.h"

/**
 * 
 */

enum class EBuildingTypes : unsigned char;
enum class ECharacterClassType : unsigned char;
enum class ENpcBehaviourStates : unsigned char;

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

	void OnLevelLoaded();
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(BlueprintCallable)
	void SpawnOnClick();

	void SetNpcBehaviourState(int UniqueID,  ENpcBehaviourStates State, ECharacterClassType CharacterType);
	struct FBuildingStatsBase* CreateBuildingStats(EBuildingTypes BuildingType);
	void UpgradeBuilding(EBuildingTypes BuildingType, bool& OutIsNextUpdateAvailable);
	TArray<FTableRowBase*> GetBuildingUpgradeRowsByType(EBuildingTypes BuildingType);

	static void UpdateTownHud(UObject* Caller);
	


	UPROPERTY(EditAnywhere, Category = Building)
	UDataTable* BuildingDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	UDataTable* RecruitBuildingUpgradeTable;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TSubclassOf<class ATownNpcCharacter> NpcCharacterBlueprint;

	UPROPERTY(BlueprintReadWrite)
	class UTownHudWidget* HudWidgetInstance;
	UPROPERTY(BlueprintReadWrite)
	class UOwnedCharactersDroppableWidget* OwnedCharsDroppableWidgetInstance;

	UPROPERTY()
	class UTownNpcManager* NpcManager;

	UPROPERTY(BlueprintReadWrite)
	class UOverlay* HudBuildingWidgetsContainerOverlay;

	TArray<struct FCharacterStats*> OwnedCharacters;
	TMap<EBuildingTypes, struct FBuildingStatsBase*> BuildingStatsMap;
	
};
