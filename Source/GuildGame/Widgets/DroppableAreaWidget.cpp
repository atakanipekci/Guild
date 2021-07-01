// Fill out your copyright notice in the Description page of Project Settings.


#include "DroppableAreaWidget.h"


#include "DraggedCharacterWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/PanelWidget.h"
#include "GuildGame/Managers/ImageManager.h"
#include "GuildGame/Managers/WidgetManager.h"

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
    if(ContentPanel)
	{
		for (int i = 0; i < ContentPanel->GetChildrenCount(); ++i)
		{
			UDraggedCharacterWidget* Child = Cast<UDraggedCharacterWidget>(ContentPanel->GetChildAt(i));
			if(Child)
			{
				Child->SetPreviousChildIndex(i);
			}
		}
	}
}

void UDroppableAreaWidget::RemoveFromDroppableArea(UDraggedCharacterWidget* DraggableToRemove)
{
    if(DraggableToRemove && ContentPanel && ContentPanel->HasChild(DraggableToRemove))
	{
		ContentPanel->RemoveChild(DraggableToRemove);
    	UpdateChildIndices();
	}
}

void UDroppableAreaWidget::OnDragEnterIntoAnotherDraggable(UDraggedCharacterWidget* Dragged,
    UDraggedCharacterWidget* To)
{
}

void UDroppableAreaWidget::OnChildDraggableDragCancelled(UDraggedCharacterWidget* Dragged, int PreviousChildIndex)
{
    if(ContentPanel)
    {
        Dragged->SetVisibility(ESlateVisibility::Visible);

        if(ContentPanel->HasChild(Dragged) == false)
        {
            ContentPanel->InsertChildAt(PreviousChildIndex, Dragged);
            UpdateChildIndices();
            
            TArray<UWidget*> Childs;
            UPanelWidget* Parent = Dragged->GetParent();

            if(Parent)
            {
                for (int i = 0; i < Parent->GetChildrenCount(); ++i)
                {
                    UWidget* Child = Parent->GetChildAt(i);
                    Childs.Add(Child);
                }
                Parent->ClearChildren();
                for (int i = 0; i < Childs.Num(); ++i)
                {
                    Parent->AddChild(Childs[i]);
                }
            }
        }
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DRAG CANCEL INDEX %d"), PreviousChildIndex ));
    }
}

UDraggedCharacterWidget* UDroppableAreaWidget::CreateAndAddChildToContentPanel(FCharacterStats* Stat, EWidgetKeys WidgetKey)
{
	if(Stat == nullptr || ContentPanel == nullptr) return nullptr;

	UDraggedCharacterWidget* NewWidget = CreateWidget<UDraggedCharacterWidget>(this->GetWorld(), AWidgetManager::GetWidget(WidgetKey, GetWorld()));
	if(NewWidget)
	{
		ContentPanel->AddChild(NewWidget);
		NewWidget->SetOwnerAreaWidget(this);
		NewWidget->SetStat(Stat);
		
		if(Stat)
			ImageManager::SetPortraitTextureByClass(Stat->ClassType, NewWidget->Portrait);

		UpdateChildIndices();


	}
	return  NewWidget;
}

UDraggedCharacterWidget* UDroppableAreaWidget::CreateAndAddChildToContentPanel(FCharacterStats* Stat,
	EWidgetKeys WidgetKey, int Row)
{
	return  nullptr;
}

