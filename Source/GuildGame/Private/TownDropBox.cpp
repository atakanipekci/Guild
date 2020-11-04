// Fill out your copyright notice in the Description page of Project Settings.


#include "TownDropBox.h"


#include "TownDefaultPawn.h"
#include "TownGameInstance.h"
#include "TownGameModeBase.h"
#include "TownScrollBox.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/CanvasPanel.h"
#include "Components/ScaleBox.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"


void UTownDropBox::NativeConstruct()
{
    Super::NativeConstruct();
	ScrollType = EScrollBoxType::SquadDrop;
}

bool UTownDropBox::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
   if(ScrollType == EScrollBoxType::SquadDrop)
	{
		UTownScrollBoxItem* ScrollItem = Cast<UTownScrollBoxItem>(InOperation->Payload);
		if(ScrollItem != nullptr)
		{
			if(ScrollItem->ScrollType == EScrollBoxType::Menu)
			{
				ScrollItem->RemoveFromParent();

				ATownDefaultPawn* Pawn = Cast<ATownDefaultPawn>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
				ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this->GetWorld()));
				
				if(Pawn && ContentScaleBox && GameMode)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Scroll First"));

					FScrollItem* ScrollDat = Pawn->ScrollItemsMap.Find(ScrollType);
					if(ScrollDat)
					{
						UTownScrollBoxItem* NewItem =  ScrollDat->ScrollItem;
						if(NewItem != nullptr)
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("New Scroll"));
				
							UTownScrollBoxItem* NewWidget = CreateWidget<UTownScrollBoxItem>(this->GetWorld(), NewItem->GetClass());

							if(ContentScaleBox->GetChildAt(0) != nullptr)
							{
								UTownScrollBoxItem* PrevItem = Cast<UTownScrollBoxItem>(ContentScaleBox->GetChildAt(0));
								if(PrevItem)
								{
									if(PrevItem->PrevParentType == EScrollBoxType::Menu)
									{
										if(PrevItem->PrevParentWidget)
										{
											PrevItem->RemoveFromParent();
											PrevItem->PrevParentWidget->AddChild(PrevItem);
										}
									}
								}
							}
							ContentScaleBox->AddChild(NewWidget);
							NewWidget->Stat = ScrollItem->Stat;
							
							
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
							return true;
						}
					}
				}
			}
			else if(ScrollItem->ScrollType == EScrollBoxType::SquadDrop)
			{
				ScrollItem->RemoveFromParent();
				ContentScaleBox->AddChild(ScrollItem);
				return true;
			}
			else
			{
				return  false;
			}

			

		}
		//ScrollBox->AddChild();
		
	}
	
    return  false;
}