// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroppableAreaWidget.h"
#include "OwnedCharactersDroppableWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UOwnedCharactersDroppableWidget : public UDroppableAreaWidget
{
	GENERATED_BODY()
	
	public:
		virtual void NativeConstruct() override;
		virtual void DropFrom(UDraggedCharacterWidget* DraggedWidget) override;
		virtual bool DropTo(UDraggedCharacterWidget* DraggedWidget) override;
		virtual void RemoveFromDroppableArea(UDraggedCharacterWidget* DraggableToRemove) override;
		virtual void OnDragEnterIntoAnotherDraggable(UDraggedCharacterWidget* Dragged, UDraggedCharacterWidget* To) override;
		virtual void OnChildDraggableDragCancelled(UDraggedCharacterWidget* Dragged, int PreviousChildIndex) override;
		virtual UDraggedCharacterWidget* CreateAndAddChildToContentPanel(FCharacterStats* Stat, EWidgetKeys WidgetKey, int Row) override;
	
	    void Refresh() ;
		void AddToScrollGrid(UDraggedCharacterWidget* WidgetToAdd, int Index, FCharacterStats* Stat);
		UDraggedCharacterWidget* SpawnCopyCharacterWidget(UDraggedCharacterWidget* DraggedWidget);


		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* ScrollBox;
		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UUniformGridPanel* ScrollUniformGridPanel;
		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USpacer* SpacerForUpdate;

		UPROPERTY()
		class ATownGameModeBase* GameMode;
	
};
