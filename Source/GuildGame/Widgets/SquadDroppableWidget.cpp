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
	RemoveFromDroppableArea(DraggedWidget);
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
							DropFrom(PrevItem);
							DraggedWidget->OwnerDroppableArea->DropTo(PrevItem);
						}
					}
    				
					UDraggedCharacterWidget* NewWidget = CreateAndAddChildToContentPanel(DraggedWidget->Stat, EWidgetKeys::DraggedSquadWidget);
					
					if(NewWidget)
					{
						//DraggedWidget->OwnerDroppableArea->RemoveFromDroppableArea(DraggedWidget);

						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("After Remove Team array count  %d"), GameMode->OwnedCharacters.Num()));
						// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SQuad count  %d"), GameInstance->SquadCharacters.Num()));
						return true;
					}
				}
    		}
    		else if(DraggedWidget->OwnerDroppableArea->AreaType ==  EDroppableAreaType::SquadCharacters)
    		{
    			if(ContentScaleBox && DraggedWidget)
    			{
    				DraggedWidget->OwnerDroppableArea->RemoveFromDroppableArea(DraggedWidget);
    				if(ContentScaleBox->GetChildAt(0) != nullptr)
					{
						UDraggedCharacterWidget* PrevItem = Cast<UDraggedCharacterWidget>(ContentScaleBox->GetChildAt(0));
						if(PrevItem && PrevItem->OwnerDroppableArea)
						{
							DropFrom(PrevItem);
							DraggedWidget->OwnerDroppableArea->DropTo(PrevItem);
						}
					}
    				
    				ContentScaleBox->AddChild(DraggedWidget);
    				DraggedWidget->SetOwnerAreaWidget(this);

    				DraggedWidget->SetVisibility(ESlateVisibility::Visible);
    			}
    			return true;
    		}
    	}
    }

	return false;
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


