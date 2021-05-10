// Fill out your copyright notice in the Description page of Project Settings.


#include "RecruitableCharactersDroppableWidget.h"



#include "CharacterStats.h"
#include "DraggedCharacterWidget.h"
#include "GuildGame/Managers/ImageManager.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "GuildGame/Town/Navigation/TownNpcCharacter.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

void URecruitableCharactersDroppableWidget::NativeConstruct()
{
    Super::NativeConstruct();
    AreaType = EDroppableAreaType::RecruitableCharacters;
    WidgetType = EDroppableWidgetType::Scroller;
	ContentPanel = ScrollBox;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CONSTRUCTOR %d"), 4));

	if(ScrollBox)
	{
		for (int i = 0; i < 4; ++i)
		{
			UDraggedCharacterWidget* NewWidget = CreateWidget<UDraggedCharacterWidget>(this->GetWorld(), WidgetManager::GetWidget(EWidgetKeys::DraggedOwnedWidget));
			if(NewWidget)
			{
				ScrollBox->AddChild(NewWidget);
				NewWidget->SetOwnerAreaWidget(this);

				ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this));

				 if(GameMode->CharactersTable && NewWidget->Stat == nullptr)
			    {
			        const FString Context(TEXT("CONTEXT DATATABLE TEXT"));
				 	FString CharacterType = "Knight";

				 	if(i >=2)
				 	{
				 		CharacterType = "Mage";
					
				 	}
			        const FName Row = *(CharacterType);

			        FCharacterStats* CharacterData = GameMode->CharactersTable->FindRow<FCharacterStats>(Row, Context, true);

			        if(CharacterData)
			        {
			        	int UniqueID = WidgetManager::GetAndSetDraggableSpawnCount();
			            FCharacterStats* CopyStruct = new FCharacterStats(*CharacterData);
			            CopyStruct->Price = 50;
			        	CopyStruct->UniqueID = UniqueID;
			        	CopyStruct->TownNpcBehaviourState = ENpcBehaviourStates::WalkingAround;

			        	NewWidget->SetStat(CopyStruct);

			        	ImageManager::SetPortraitTextureByClass(CopyStruct->ClassType, NewWidget->Portrait);
			            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DRAGGABLE NAME %s"), *this->GetName()));
			        }

			    }
			}
		}
	}
	
}

void URecruitableCharactersDroppableWidget::DropFrom(UDraggedCharacterWidget* DraggedWidget)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DROPPPED FROM RECRUIT"));
}

bool URecruitableCharactersDroppableWidget::DropTo(UDraggedCharacterWidget* DraggedWidget)
{
   

	return false;
}