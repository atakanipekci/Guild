// Fill out your copyright notice in the Description page of Project Settings.


#include "TownScrollBox.h"



#include "TownDefaultPawn.h"
#include "TownGameInstance.h"
#include "TownGameModeBase.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

class ATownDefaultPawn;

void UTownScrollBox::NativeConstruct()
{
    Super::NativeConstruct();

}

void UTownScrollBox::SetScrollType(EScrollBoxType Type)
{
	ScrollType = Type;

	if(Type == EScrollBoxType::Menu)
	{
		
	}
	else if(Type == EScrollBoxType::Recruit)
	{
		 ATownDefaultPawn* Pawn = Cast<ATownDefaultPawn>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
		if(Pawn && ScrollBox)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Scroll First"));

			FScrollItem* ScrollDat = Pawn->ScrollItemsMap.Find(Type);
			if(ScrollDat)
			{
				UTownScrollBoxItem* NewItem =  ScrollDat->ScrollItem;
				if(NewItem != nullptr)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("New Scroll"));
					for (int i = 0; i < 2; ++i)
					{
						UTownScrollBoxItem* NewWidget = CreateWidget<UTownScrollBoxItem>(this->GetWorld(), NewItem->GetClass());
						ScrollBox->AddChild(NewWidget);
					}
					
				}
			}
		}

	}
}


bool UTownScrollBox::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{	
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if(ScrollType == EScrollBoxType::Menu)
	{
		UTownScrollBoxItem* ScrollItem = Cast<UTownScrollBoxItem>(InOperation->Payload);
		if(ScrollItem != nullptr)
		{
			if(ScrollItem->ScrollType == EScrollBoxType::Recruit || ScrollItem->ScrollType == EScrollBoxType::SquadDrop)
			{
				ScrollItem->RemoveFromParent();

				ATownDefaultPawn* Pawn = Cast<ATownDefaultPawn>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
				ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this->GetWorld()));
				
				if(Pawn && ScrollBox && GameMode)
				{

					FScrollItem* ScrollDat = Pawn->ScrollItemsMap.Find(ScrollType);
					if(ScrollDat)
					{

						UTownScrollBoxItem* NewItem =  ScrollDat->ScrollItem;
						if(NewItem != nullptr)
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("New Scroll"));
				
							UTownScrollBoxItem* NewWidget = CreateWidget<UTownScrollBoxItem>(this->GetWorld(), NewItem->GetClass());
							ScrollBox->AddChild(NewWidget);
							NewWidget->Stat = ScrollItem->Stat;

							if(GameMode->OwnedCharacters.Find(ScrollItem->Stat) == INDEX_NONE)
							{
								GameMode->OwnedCharacters.Add(ScrollItem->Stat);
								if(ScrollItem->ScrollType == EScrollBoxType::SquadDrop)
								{
									UTownGameInstance* GameInstance = Cast<UTownGameInstance>(UGameplayStatics::GetGameInstance(this->GetWorld()));

									if(GameInstance->SquadCharacters.Find(ScrollItem->Stat) != INDEX_NONE)
									{
										GameInstance->SquadCharacters.Remove(ScrollItem->Stat);
									}
									GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Squad count  %d"), GameInstance->SquadCharacters.Num()));

								}
								GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Team array count  %d"), GameMode->OwnedCharacters.Num()));
							}

							return true;
						}
					}
				}
			}
			else if(ScrollItem->ScrollType == EScrollBoxType::Menu)
			{
				ScrollItem->RemoveFromParent();
				ScrollBox->AddChild(ScrollItem);
				return true;

			}
		}
		//ScrollBox->AddChild();
		
	}
	else if(ScrollType == EScrollBoxType::Recruit)
	{
		UTownScrollBoxItem* ScrollItem = Cast<UTownScrollBoxItem>(InOperation->Payload);
		if(ScrollItem != nullptr)
		{
			if(ScrollItem->ScrollType == EScrollBoxType::Recruit)
			{
				ScrollItem->RemoveFromParent();
				ScrollBox->AddChild(ScrollItem);
				return true;
			}
		}
	}
	return false;
}  