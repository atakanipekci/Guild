// Fill out your copyright notice in the Description page of Project Settings.


#include "DroppableAreaWidget.h"


#include "DraggedCharacterWidget.h"
#include "Blueprint/DragDropOperation.h"

bool UDroppableAreaWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                        UDragDropOperation* InOperation)
{
    if(InOperation->Payload)
    {
        UDraggedCharacterWidget* DraggedWidget = Cast<UDraggedCharacterWidget>(InOperation->Payload);
        if(DraggedWidget)
        {
            const bool bIsSuccessfull = DropTo(DraggedWidget);
            if(DraggedWidget->OwnerDroppableArea && bIsSuccessfull)
            {
                DraggedWidget->OwnerDroppableArea->DropFrom(DraggedWidget);
            }

            return bIsSuccessfull;
        }
    }
    return false;
}

void UDroppableAreaWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if(InOperation->Payload)
    {
        UDraggedCharacterWidget* DraggedWidget = Cast<UDraggedCharacterWidget>(InOperation->Payload);
        if(DraggedWidget)
        {
            DraggedWidget->OnDragLeaveFromArea(this);
        }
    }
}

bool UDroppableAreaWidget::DropTo(UDraggedCharacterWidget* DraggedWidget)
{
    return true;
}

void UDroppableAreaWidget::DropFrom(UDraggedCharacterWidget* DraggedWidget)
{
}

void UDroppableAreaWidget::UpdateChildIndices()
{
    
}

