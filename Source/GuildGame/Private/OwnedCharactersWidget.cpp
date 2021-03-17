// Fill out your copyright notice in the Description page of Project Settings.


#include "OwnedCharactersWidget.h"



#include "CharacterStats.h"
#include "DraggedCharacterWidget.h"
#include "TownGameInstance.h"
#include "TownGameModeBase.h"
#include "TownNpcCharacter.h"

#include "WidgetManager.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"


void UOwnedCharactersWidget::NativeConstruct()
{
    Super::NativeConstruct();
    AreaType = EDroppableAreaType::OwnedCharacters;
    WidgetType = EDroppableWidgetType::Scroller;
	ContentPanel = ScrollBox;
}

void UOwnedCharactersWidget::DropFrom(UDraggedCharacterWidget* DraggedWidget)
{
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DROPPPED FROM OWNED"));

}

bool UOwnedCharactersWidget::DropTo(UDraggedCharacterWidget* DraggedWidget)
{
    if(DraggedWidget)
    {
    	if(DraggedWidget->OwnerDroppableArea)
    	{
    		if(DraggedWidget->OwnerDroppableArea->AreaType ==  EDroppableAreaType::RecruitableCharacters)
    		{
    			if(ScrollBox)
				{
					UDraggedCharacterWidget* NewWidget = CreateChildWidget(DraggedWidget);
					if(NewWidget)
					{
						DraggedWidget->RemoveFromRoot();

						ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this->GetWorld()));

						if(GameMode)
						{
							if(GameMode->OwnedCharacters.Find(NewWidget->Stat) == INDEX_NONE)
							{
								GameMode->OwnedCharacters.Add(NewWidget->Stat);
								//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Team array count  %d"), GameMode->OwnedCharacters.Num()));
							}
							UE_LOG(LogTemp, Warning, TEXT("DROP 1 "));
							if(GameMode->NpcManager)
							{
								UE_LOG(LogTemp, Warning, TEXT("DROP 2 "));
								GameMode->SetNpcBehaviourState(NewWidget->Stat->UniqueID, ENpcBehaviourStates::WalkingAround, NewWidget->Stat->ClassType);
							}
						}
						
						return true;
					}
				}
    		}
    		else if(DraggedWidget->OwnerDroppableArea->AreaType ==  EDroppableAreaType::SquadCharacters)
    		{
    			if(ScrollBox)
				{
					UDraggedCharacterWidget* NewWidget = CreateChildWidget(DraggedWidget);

					if(NewWidget)
					{
						DraggedWidget->RemoveFromRoot();

						ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this->GetWorld()));
						if(GameMode->OwnedCharacters.Find(NewWidget->Stat) == INDEX_NONE)
						{
							GameMode->OwnedCharacters.Add(NewWidget->Stat);
							
							UTownGameInstance* GameInstance = Cast<UTownGameInstance>(UGameplayStatics::GetGameInstance(this->GetWorld()));

							if(GameInstance->SquadCharacters.Find(NewWidget->Stat) != INDEX_NONE)
							{
								GameInstance->SquadCharacters.Remove(NewWidget->Stat);
							}
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Squad count  %d"), GameInstance->SquadCharacters.Num()));
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Team array count  %d"), GameMode->OwnedCharacters.Num()));
						}
						return true;
					}
				}
    		}
    	}
    }

	return false;
}

UDraggedCharacterWidget* UOwnedCharactersWidget::CreateChildWidget(UDraggedCharacterWidget* DraggedWidget)
{
	UDraggedCharacterWidget* NewWidget = CreateWidget<UDraggedCharacterWidget>(this->GetWorld(), WidgetManager::GetWidget(EWidgetKeys::DraggedOwnedWidget));
	if(NewWidget && ScrollBox)
	{
		//NewWidget->SetVisibility(ESlateVisibility::Hidden);
		if(ScrollBox->HasChild(DraggedWidget))
		{
			ScrollBox->InsertChildAt(ScrollBox->GetChildIndex(DraggedWidget), NewWidget);
			
			TArray<UWidget*> Childs;
            UPanelWidget* Parent = ScrollBox;
			
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
		else
		{
			ScrollBox->AddChild(NewWidget);
		}
		NewWidget->SetOwnerAreaWidget(this);

		NewWidget->SetStat(DraggedWidget->Stat);
		NewWidget->LatestChildIndex = ScrollBox->GetChildIndex(NewWidget);

		DraggedWidget->RemoveFromParent();

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("OWNEd added index %d"), ScrollBox->GetChildIndex(NewWidget)));

	}

	return NewWidget;
}

	
