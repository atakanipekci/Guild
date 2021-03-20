// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Engine/DataTable.h"
#include "WidgetManager.generated.h"


/**
 * 
 */

UENUM()
enum class EWidgetKeys: uint8
{
	YesOrNo,
	DraggedOwnedWidget,
	DraggedSquadWidget,
	TavernMenuWidget,
	RecruitMenuWidget,
	CharacterDetail
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

class GUILDGAME_API WidgetManager
{
	public:

		static void SetWidgetTable(UDataTable* Table);
		static TSubclassOf<UUserWidget> GetWidget(const EWidgetKeys Key);
		static FString GetWidgetRowName(const EWidgetKeys Key);
		static void SetWidgetInstance(const EWidgetKeys Key, UUserWidget* Widget);
		static UUserWidget* GetWidgetInstance(const EWidgetKeys Key);

		static void SetSequenceTable(UDataTable* Table);
		static FString GetSequenceRowName(ESequenceKeys Key);
		static ULevelSequence* GetSequence(ESequenceKeys Key);
		static void ResetWidgetInstances();
		
		static int GetAndSetDraggableSpawnCount();

		
		static UDataTable* WidgetsDataTable;
		static UDataTable* SequenceDataTable;
		static TMap<EWidgetKeys, UUserWidget* > WidgetInstanceMap;


		static int SpawnedDraggableWidgetCount;
	


}; 