// Fill out your copyright notice in the Description page of Project Settings.


#include "RecruitableCharactersDroppableWidget.h"



#include "GuildGame/Characters/CharacterStats.h"
#include "DraggedCharacterWidget.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "GuildGame/Managers/CharacterGenerationManager.h"

void URecruitableCharactersDroppableWidget::NativeConstruct()
{
    Super::NativeConstruct();
    AreaType = EDroppableAreaType::RecruitableCharacters;
	ContentPanel = ScrollBox;
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CONSTRUCTOR %d"), 4));

	RefreshRecruitables();
	
}

void URecruitableCharactersDroppableWidget::DropFrom(UDraggedCharacterWidget* DraggedWidget)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DROPPPED FROM RECRUIT"));
	RemoveFromDroppableArea(DraggedWidget);
}

bool URecruitableCharactersDroppableWidget::DropTo(UDraggedCharacterWidget* DraggedWidget)
{
	return false;
}

void URecruitableCharactersDroppableWidget::RefreshRecruitables()
{
	if(ScrollBox)
	{
		for (int j = 0; j < ScrollBox->GetChildrenCount(); ++j)
		{
			UDraggedCharacterWidget* Child = Cast<UDraggedCharacterWidget>(ScrollBox->GetChildAt(j));
			if(Child)
			{
				delete(Child->Stat);
			}
		}
		ScrollBox->ClearChildren();
		
		
		for (int i = 0; i < 4; ++i)
		{
			FCharacterStats* NewCharacter = ACharacterGenerationManager::CreateRandomCharacter(GetWorld());
			if(NewCharacter)
			{
				CreateAndAddChildToContentPanel(NewCharacter, EWidgetKeys::DraggedRecruitWidget);
			}
		}
	}
}
