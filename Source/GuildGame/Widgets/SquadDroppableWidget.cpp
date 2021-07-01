// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadDroppableWidget.h"
#include "DraggedCharacterWidget.h"
#include "GuildGame/Managers/ImageManager.h"
#include "GuildGameInstance.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "Components/ScaleBox.h"
#include "Kismet/GameplayStatics.h"

void USquadDroppableWidget::NativeConstruct()
{
    Super::NativeConstruct();
    AreaType = EDroppableAreaType::SquadCharacters;
	ContentPanel = ContentScaleBox;
	
}

void USquadDroppableWidget::DropFrom(UDraggedCharacterWidget* DraggedWidget)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DROPPPED FROM SQUAD"));
	//RemoveFromDroppableArea(DraggedWidget);
}

bool USquadDroppableWidget::DropTo(UDraggedCharacterWidget* DraggedWidget)
{
    if(DraggedWidget)
    {
    	if(DraggedWidget->OwnerDroppableArea)
    	{
    		if(DraggedWidget->OwnerDroppableArea->AreaType ==  EDroppableAreaType::OwnedCharacters)
    		{
    			if(ContentScaleBox)
				{
    				if(ContentScaleBox->GetChildAt(0) != nullptr)
					{
						UDraggedCharacterWidget* PrevItem = Cast<UDraggedCharacterWidget>(ContentScaleBox->GetChildAt(0));
						if(PrevItem && PrevItem->OwnerDroppableArea)
						{
							RemoveFromDroppableArea(PrevItem);
							DraggedWidget->OwnerDroppableArea->DropTo(PrevItem);
						}
					}
    				
					UDraggedCharacterWidget* NewWidget = CreateAndAddChildToContentPanel(DraggedWidget->Stat, EWidgetKeys::DraggedSquadWidget);
					
					if(NewWidget)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("After Remove Team array count  %d"), GameMode->OwnedCharacters.Num()));
						// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SQuad count  %d"), GameInstance->SquadCharacters.Num()));
						return true;
					}
				}
    		}
    		else if(DraggedWidget->OwnerDroppableArea->AreaType ==  EDroppableAreaType::SquadCharacters)
    		{
    			USquadDroppableWidget* FromArea = Cast<USquadDroppableWidget>(DraggedWidget->OwnerDroppableArea);
    			FCharacterStats* DraggedStat = DraggedWidget->Stat;
    			
    			if(ContentScaleBox && FromArea && FromArea != this && DraggedStat)
    			{
    				DraggedWidget->RemoveFromParent();
    				if(ContentScaleBox->GetChildAt(0) != nullptr)
					{
						UDraggedCharacterWidget* PrevItem = Cast<UDraggedCharacterWidget>(ContentScaleBox->GetChildAt(0));
						if(PrevItem)
						{
							FCharacterStats* PrevStat = PrevItem->Stat;
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SQUAD SWAp")));
							PrevItem->RemoveFromParent();
							if(PrevStat)
							{
								FromArea->CreateAndAddChildToContentPanel(PrevStat, EWidgetKeys::DraggedSquadWidget);
							}
						}
					}

    				UDraggedCharacterWidget* NewWidget = CreateAndAddChildToContentPanel(DraggedStat, EWidgetKeys::DraggedSquadWidget);
					
					if(NewWidget)
					{
						return true;
					}
    			}
    		}
    	}
    }

	return false;
}

void USquadDroppableWidget::RemoveFromDroppableArea(UDraggedCharacterWidget* DraggableToRemove)
{
	Super::RemoveFromDroppableArea(DraggableToRemove);

	UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(this->GetWorld()));
	if(DraggableToRemove && GameInstance->SquadCharacters.Contains(DraggableToRemove->Stat))
	{
		GameInstance->SquadCharacters.Remove(DraggableToRemove->Stat);
	}
}

UDraggedCharacterWidget* USquadDroppableWidget::CreateAndAddChildToContentPanel(FCharacterStats* Stat, EWidgetKeys WidgetKey)
{
	UDraggedCharacterWidget* NewWidget = Super::CreateAndAddChildToContentPanel(Stat, WidgetKey);
	if(NewWidget && NewWidget->Stat)
	{
		UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(this->GetWorld()));
		if(GameInstance->SquadCharacters.Contains(NewWidget->Stat) == false)
		{
			GameInstance->SquadCharacters.Add(NewWidget->Stat);
		}
		NewWidget->SetVisibility(ESlateVisibility::Visible);
	}
	
	return NewWidget;
}


