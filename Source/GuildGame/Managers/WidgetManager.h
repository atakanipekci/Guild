// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Blueprint/UserWidgetPool.h"
#include "Engine/DataTable.h"
#include "WidgetManager.generated.h"


/**
 * 
 */

enum class ECharacterClassType : unsigned char;

UENUM()
enum class EWidgetKeys: uint8
{
	YesOrNo,
	DraggedOwnedWidget,
	DraggedSquadWidget,
	DraggedRecruitWidget,
	TavernMenuWidget,
	RecruitMenuWidget,
	RecruitUpgradeWidget,
	CharacterDetail,
	GuildDetail,
	TownHud,
	BattleHud,
	RecruitBuildingUpgTooltip,
	KnightSkills,
	MageSkills,
	ArcherSkills,
	CharacterSkillTooltip,
	StatusEffectNodeWidget,
	StatusEffectTooltip,
	StatusEffectStackableTooltip,
	TurnInfoNode
};

UENUM()
enum class ESequenceKeys: uint8
{
	TavernMenu,
	RecruitMenu
};

USTRUCT(BlueprintType)
struct FWidgetsDataTable : public FTableRowBase
{
    GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Widget;
};

USTRUCT(BlueprintType)
struct FSequenceDataTable : public FTableRowBase
{
    GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULevelSequence* SequenceAsset;
};

UCLASS()
class GUILDGAME_API AWidgetManager : public AActor
{
	GENERATED_BODY()
	
	public:

		virtual void BeginDestroy() override;
	
		static AWidgetManager* Instance;
		UPROPERTY()
		UDataTable* WidgetsDataTable;
		UPROPERTY()
		UDataTable* SequenceDataTable;

		UPROPERTY()
		TMap<EWidgetKeys, UUserWidget* > WidgetInstanceMap;

		static void CreateManagerInstance(UWorld* World);
	
		static TSubclassOf<UUserWidget> GetWidget(const EWidgetKeys Key, UWorld* World);
		static FString GetWidgetRowName(const EWidgetKeys Key);
		static void SetWidgetInstance(const EWidgetKeys Key, UUserWidget* Widget, UWorld* World);
		static UUserWidget* GetWidgetInstance(const EWidgetKeys Key, UWorld* World);
		static UUserWidget* GetOrCreateWidgetInstance(const EWidgetKeys Key, UWorld* World);
		static UUserWidget* CreateWidgetInstance(const EWidgetKeys Key, UWorld* World);
		static UUserWidget* GetSkillsWidgetByType(ECharacterClassType ClassType, UWorld* World);
	
		static FString GetSequenceRowName(ESequenceKeys Key);
		static ULevelSequence* GetSequence(ESequenceKeys Key, UWorld* World);
		
		static int IncrementSpawnedDraggableCount();
		static int SpawnedDraggableWidgetCount;

}; 