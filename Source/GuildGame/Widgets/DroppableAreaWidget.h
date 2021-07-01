// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DroppableAreaWidget.generated.h"

enum class EWidgetKeys : uint8;
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
	virtual void RemoveFromDroppableArea(UDraggedCharacterWidget* DraggableToRemove);
	virtual void OnDragEnterIntoAnotherDraggable(UDraggedCharacterWidget* Dragged, UDraggedCharacterWidget* To);
	virtual void OnChildDraggableDragCancelled(UDraggedCharacterWidget* Dragged, int PreviousChildIndex);
	virtual UDraggedCharacterWidget* CreateAndAddChildToContentPanel(struct FCharacterStats* Stat, EWidgetKeys WidgetKey);
	virtual UDraggedCharacterWidget* CreateAndAddChildToContentPanel(struct FCharacterStats* Stat, EWidgetKeys WidgetKey, int Row);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPanelWidget* ContentPanel;
	EDroppableAreaType AreaType;
};
