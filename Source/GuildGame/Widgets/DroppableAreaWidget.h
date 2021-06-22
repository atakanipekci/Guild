// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DroppableAreaWidget.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDroppableAreaType: uint8
{
	None,
	OwnedCharacters,
	RecruitableCharacters,
	SquadCharacters
};

UENUM(BlueprintType)
enum class EDroppableWidgetType: uint8
{
	None,
	Scroller,
	Dropbox
};

UCLASS()
class GUILDGAME_API UDroppableAreaWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation) override;

	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


	public:
	
	virtual bool DropTo(class UDraggedCharacterWidget* DraggedWidget);
	virtual void DropFrom( UDraggedCharacterWidget* DraggedWidget);
	virtual void UpdateChildIndices();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPanelWidget* ContentPanel;
	EDroppableAreaType AreaType;
	EDroppableWidgetType WidgetType;
};
