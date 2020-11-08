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
	DraggedSquadWidget
};

USTRUCT(BlueprintType)
struct FWidgetsDataTable : public FTableRowBase
{
    GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Widget;
};

class GUILDGAME_API WidgetManager
{
	public:
		static UDataTable* WidgetsDataTable;

		static void SetWidgetTable(UDataTable* Table);

		static TSubclassOf<UUserWidget> GetWidget(const EWidgetKeys Key);

		static FString GetRowName(const EWidgetKeys Key);

}; 