// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "TownGameModeBase.generated.h"

/**
 * 
 */

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
	

	UPROPERTY(EditAnywhere, Category = Building)
	UDataTable* BuildingDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	UDataTable* NpcTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TSubclassOf<class ATownNpcCharacter> NpcCharacterBlueprint;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	// TSubclassOf<class ATownNpcPawn> NpcPawnBlueprint;

	UPROPERTY(BlueprintReadWrite)
	class UTownHudWidget* MenuWidget;
	UPROPERTY(BlueprintReadWrite)
	class UOwnedCharactersDroppableWidget* OwnedCharactersWidget;

	UPROPERTY()
	class UTownNpcManager* NpcManager;

	UPROPERTY(BlueprintReadWrite)
	class UOverlay* MenuUiContainerOverlay;

	TArray<struct FCharacterStats*> OwnedCharacters;
	
	static void UpdateUI(UObject* Caller);
};
