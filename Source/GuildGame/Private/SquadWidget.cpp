// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadWidget.h"


#include "DraggedCharacterWidget.h"
#include "ImageManager.h"
#include "TownGameInstance.h"
#include "TownGameModeBase.h"
#include "WidgetManager.h"
#include "Components/ScaleBox.h"
#include "Kismet/GameplayStatics.h"

void USquadWidget::NativeConstruct()
{
    Super::NativeConstruct();
    AreaType = EDroppableAreaType::SquadCharacters;
    WidgetType = EDroppableWidgetType::Scroller;
	ContentPanel = ContentScaleBox;
	
}

void USquadWidget::DropFrom(UDraggedCharacterWidget* DraggedWidget)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DROPPPED FROM SQUAD"));
}

bool USquadWidget::DropTo(UDraggedCharacterWidget* DraggedWidget)
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
						if(PrevItem)
						{
							PrevItem->RemoveFromParent();
							DraggedWidget->OwnerDroppableArea->DropTo(PrevItem);
						}
					}
    				
					UDraggedCharacterWidget* NewWidget = CreateChildWidget(DraggedWidget);
					
					if(NewWidget)
					{
						DraggedWidget->RemoveFromRoot();
							
						ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this->GetWorld()));
						if(GameMode->OwnedCharacters.Find(NewWidget->Stat) != INDEX_NONE)
						{
							GameMode->OwnedCharacters.Remove(NewWidget->Stat);

							UTownGameInstance* GameInstance = Cast<UTownGameInstance>(UGameplayStatics::GetGameInstance(this->GetWorld()));
							if(GameInstance->SquadCharacters.Find(NewWidget->Stat) == INDEX_NONE)
							{
								GameInstance->SquadCharacters.Add(NewWidget->Stat);
							}
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("After Remove Team array count  %d"), GameMode->OwnedCharacters.Num()));
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SQuad count  %d"), GameInstance->SquadCharacters.Num()));
						}
						NewWidget->SetVisibility(ESlateVisibility::Visible);
						return true;
					}
				}
    		}
    		else if(DraggedWidget->OwnerDroppableArea->AreaType ==  EDroppableAreaType::SquadCharacters)
    		{
    			if(ContentScaleBox && DraggedWidget)
    			{
    				DraggedWidget->RemoveFromParent();
    				if(ContentScaleBox->GetChildAt(0) != nullptr)
					{
						UDraggedCharacterWidget* PrevItem = Cast<UDraggedCharacterWidget>(ContentScaleBox->GetChildAt(0));
						if(PrevItem)
						{
							PrevItem->RemoveFromParent();
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




UDraggedCharacterWidget* USquadWidget::CreateChildWidget(UDraggedCharacterWidget* DraggedWidget)
{
	UDraggedCharacterWidget* NewWidget = CreateWidget<UDraggedCharacterWidget>(this->GetWorld(), WidgetManager::GetWidget(EWidgetKeys::DraggedSquadWidget));
	if(NewWidget && ContentScaleBox)
	{
		ContentScaleBox->AddChild(NewWidget);
		NewWidget->SetOwnerAreaWidget(this);
		NewWidget->SetStat(DraggedWidget->Stat);

		if(DraggedWidget->Stat)
			ImageManager::SetPortraitTextureByClass(DraggedWidget->Stat->ClassType, NewWidget->Portrait);
	}

	return NewWidget;
}
