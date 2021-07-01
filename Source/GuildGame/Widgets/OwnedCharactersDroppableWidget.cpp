// Fill out your copyright notice in the Description page of Project Settings.


#include "OwnedCharactersDroppableWidget.h"
#include "GuildGame/Characters/CharacterStats.h"
#include "DraggedCharacterWidget.h"
#include "GuildGameInstance.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "GuildGame/Town/Navigation/TownNpcCharacter.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "Components/ScrollBox.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "GuildGame/Managers/ImageManager.h"
#include "Kismet/GameplayStatics.h"


void UOwnedCharactersDroppableWidget::NativeConstruct()
{
    Super::NativeConstruct();
    AreaType = EDroppableAreaType::OwnedCharacters;
	if(ScrollUniformGridPanel)
	{
		ContentPanel = Cast<UPanelWidget>(ScrollUniformGridPanel);
	}
	// ContentPanel = ScrollBox;

	GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this->GetWorld()));
	if(GameMode)
	{
		GameMode->OwnedCharsDroppableWidgetInstance = this;
	}
}

void UOwnedCharactersDroppableWidget::DropFrom(UDraggedCharacterWidget* DraggedWidget)
{
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DROPPPED FROM OWNED"));
		RemoveFromDroppableArea(DraggedWidget);
}

bool UOwnedCharactersDroppableWidget::DropTo(UDraggedCharacterWidget* DraggedWidget)
{
    if(DraggedWidget)
    {
    	if(DraggedWidget->OwnerDroppableArea)
    	{
    		if(DraggedWidget->OwnerDroppableArea->AreaType ==  EDroppableAreaType::RecruitableCharacters)
    		{
				UDraggedCharacterWidget* NewWidget = SpawnCopyCharacterWidget(DraggedWidget);
    			
				if(NewWidget)
				{
					NewWidget->SetVisibility(ESlateVisibility::Visible);
					if(GameMode)
					{
						if(NewWidget->Stat)
						{
							GameMode->SetNpcBehaviourState(NewWidget->Stat->UniqueID, ENpcBehaviourStates::WalkingAround, NewWidget->Stat->ClassType);
						}
					}
					
					return true;
				}
    		}
    		else if(DraggedWidget->OwnerDroppableArea->AreaType ==  EDroppableAreaType::SquadCharacters)
    		{
				UDraggedCharacterWidget* NewWidget = SpawnCopyCharacterWidget(DraggedWidget);

				if(NewWidget)
				{
					NewWidget->SetVisibility(ESlateVisibility::Visible);
					return true;
				}
    		}
    	}
    }

	return false;
}

void UOwnedCharactersDroppableWidget::RemoveFromDroppableArea(UDraggedCharacterWidget* DraggableToRemove)
{
	if(DraggableToRemove == nullptr || ScrollUniformGridPanel == nullptr) return;


	if(ScrollUniformGridPanel->HasChild(DraggableToRemove))
	{
		int RowIndex = 0;
		UUniformGridSlot* GridSlotToRemove = Cast<UUniformGridSlot>(DraggableToRemove->Slot);
		if(GridSlotToRemove)
		{
			RowIndex = GridSlotToRemove->Row;
			DraggableToRemove->SetPreviousChildIndex(GridSlotToRemove->Row);
		}
		

		for (int i = 0; i < ScrollUniformGridPanel->GetChildrenCount(); ++i)
		{
			UWidget* Child = ScrollUniformGridPanel->GetChildAt(i);
			if(Child)
			{
				UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(Child->Slot);

				if(GridSlot && GridSlot->Row > RowIndex)
				{
					GridSlot->SetRow(GridSlot->Row - 1);
				}
			}
		}
		
		if(GameMode && DraggableToRemove->Stat && GameMode->OwnedCharacters.Contains(DraggableToRemove->Stat))
		{
			DraggableToRemove->Stat->bIsOwned = false;
			GameMode->OwnedCharacters.Remove(DraggableToRemove->Stat);
		}
		ScrollUniformGridPanel->RemoveChild(DraggableToRemove);
	}
	
	RemoveFromDroppableArea(DraggableToRemove->DraggedCopy);
	DraggableToRemove->DraggedCopy = nullptr;

}

void UOwnedCharactersDroppableWidget::Refresh()
{
	if(ScrollUniformGridPanel)
	{
		ScrollUniformGridPanel->ClearChildren();
		for (int i = 0; i < GameMode->OwnedCharacters.Num(); ++i)
		{
			CreateAndAddChildToContentPanel(GameMode->OwnedCharacters[i], EWidgetKeys::DraggedOwnedWidget, ScrollUniformGridPanel->GetChildrenCount());
		}
	}
}

void UOwnedCharactersDroppableWidget::AddToScrollGrid(UDraggedCharacterWidget* WidgetToAdd, int Index, FCharacterStats* Stat)
{
	if(ScrollUniformGridPanel && WidgetToAdd && ScrollUniformGridPanel->HasChild(WidgetToAdd) == false && Stat)
	{
		for (int i = 0; i < ScrollUniformGridPanel->GetChildrenCount(); ++i)
		{
			UWidget* Child = ScrollUniformGridPanel->GetChildAt(i);
			if(Child)
			{
				UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(Child->Slot);

				if(GridSlot && GridSlot->Row >= Index)
				{
					GridSlot->SetRow(GridSlot->Row + 1);
				}
			}
		}
		UUniformGridSlot* NewAddedSlot = ScrollUniformGridPanel->AddChildToUniformGrid(WidgetToAdd, Index);
		if(GameMode && Stat && GameMode->OwnedCharacters.Contains(Stat) == false)
		{
			Stat->bIsOwned = true;
			GameMode->OwnedCharacters.Add(Stat);
		}
		
		if(NewAddedSlot)
		{
			NewAddedSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			NewAddedSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		}
	}
}

UDraggedCharacterWidget* UOwnedCharactersDroppableWidget::SpawnCopyCharacterWidget(UDraggedCharacterWidget* DraggedWidget)
{
	if(DraggedWidget == nullptr || DraggedWidget->OwnerDroppableArea == nullptr || ScrollUniformGridPanel == nullptr) return nullptr;

	if(ScrollUniformGridPanel->HasChild(DraggedWidget))
	{
		return DraggedWidget;
	}
	else if(DraggedWidget->DraggedCopy && ScrollUniformGridPanel->HasChild(DraggedWidget->DraggedCopy))
	{
		return DraggedWidget->DraggedCopy;
	}
	else
	{
		UDraggedCharacterWidget* NewWidget = CreateAndAddChildToContentPanel(DraggedWidget->Stat, EWidgetKeys::DraggedOwnedWidget, ScrollUniformGridPanel->GetChildrenCount());
		if(NewWidget)
		{
			DraggedWidget->OwnerDroppableArea->RemoveFromDroppableArea(DraggedWidget);
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("OWNEd added index %d"), ScrollBox->GetChildIndex(NewWidget)));
		}

		return NewWidget;
	}
}

void UOwnedCharactersDroppableWidget::OnDragEnterIntoAnotherDraggable(UDraggedCharacterWidget* Dragged,
	UDraggedCharacterWidget* To)
{
	if(Dragged == nullptr || To == nullptr || ScrollUniformGridPanel == nullptr) return;

	if(ScrollUniformGridPanel->HasChild(To))
	{
		if(ScrollUniformGridPanel->HasChild(Dragged))
		{
			UUniformGridSlot* DraggedGridSlot = Cast<UUniformGridSlot>(Dragged->Slot);
			UUniformGridSlot* ToGridSlot = Cast<UUniformGridSlot>(To->Slot);
			if(DraggedGridSlot && ToGridSlot)
			{
				int TempRow = DraggedGridSlot->Row;
				DraggedGridSlot->SetRow(ToGridSlot->Row);
				ToGridSlot->SetRow(TempRow);
				if(SpacerForUpdate)
				{
					ScrollUniformGridPanel->AddChild(SpacerForUpdate);
					ScrollUniformGridPanel->RemoveChild(SpacerForUpdate);
				}
			}
		}
		else
		{
			if(Dragged->OwnerDroppableArea && Dragged->OwnerDroppableArea->AreaType == AreaType)
			{
				AddToScrollGrid(Dragged, Dragged->GetPreviousChildIndex(), Dragged->Stat);
			}
			else if(Dragged->DraggedCopy != nullptr)
			{
				UUniformGridSlot* CopyGridSlot = Cast<UUniformGridSlot>(Dragged->DraggedCopy->Slot);
				UUniformGridSlot* ToGridSlot = Cast<UUniformGridSlot>(To->Slot);
				if(CopyGridSlot && ToGridSlot)
				{
					int TempRow = CopyGridSlot->Row;
					CopyGridSlot->SetRow(ToGridSlot->Row);
					ToGridSlot->SetRow(TempRow);
					if(SpacerForUpdate)
					{
						ScrollUniformGridPanel->AddChild(SpacerForUpdate);
						ScrollUniformGridPanel->RemoveChild(SpacerForUpdate);
					}
				}
			}
			else if(Dragged->DraggedCopy == nullptr)
			{
				UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(To->Slot);
				if(GridSlot)
				{
					UDraggedCharacterWidget* CreatedCopy = CreateAndAddChildToContentPanel(Dragged->Stat, EWidgetKeys::DraggedOwnedWidget,GridSlot->Row);

					if(CreatedCopy)
					{
						CreatedCopy->SetVisibility(ESlateVisibility::Hidden);
						Dragged->DraggedCopy = CreatedCopy;
					}
				}
			}
		}
	}
}

UDraggedCharacterWidget* UOwnedCharactersDroppableWidget::CreateAndAddChildToContentPanel(FCharacterStats* Stat,
	EWidgetKeys WidgetKey, int Row)
{
	if(Stat == nullptr || ScrollUniformGridPanel == nullptr) return nullptr;
	
	UDraggedCharacterWidget* NewWidget = CreateWidget<UDraggedCharacterWidget>(this->GetWorld(), AWidgetManager::GetWidget(WidgetKey, GetWorld()));
	if(NewWidget)
	{
		AddToScrollGrid(NewWidget, Row, Stat);

		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(NewWidget->Slot);
		if(GridSlot)
		{
			GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		}
		NewWidget->SetOwnerAreaWidget(this);
		NewWidget->SetStat(Stat);

		if(Stat)
			ImageManager::SetPortraitTextureByClass(Stat->ClassType, NewWidget->Portrait);


	}
	return  NewWidget;
}

void UOwnedCharactersDroppableWidget::OnChildDraggableDragCancelled(UDraggedCharacterWidget* Dragged,
	int PreviousChildIndex)
{
    if(ScrollUniformGridPanel)
    {
        Dragged->SetVisibility(ESlateVisibility::Visible);

        if(ScrollUniformGridPanel->HasChild(Dragged) == false)
        {
            AddToScrollGrid(Dragged, PreviousChildIndex, Dragged->Stat);
        }
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DRAG CANCEL INDEX %d"), PreviousChildIndex ));
    }
}

	
