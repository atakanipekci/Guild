// Fill out your copyright notice in the Description page of Project Settings.


#include "RecruitableCharactersDroppableWidget.h"



#include "GuildGame/Characters/CharacterStats.h"
#include "DraggedCharacterWidget.h"
#include "GuildGame/Managers/ImageManager.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "GuildGame/Managers/CharacterGenerationManager.h"

void URecruitableCharactersDroppableWidget::NativeConstruct()
{
    Super::NativeConstruct();
    AreaType = EDroppableAreaType::RecruitableCharacters;
    WidgetType = EDroppableWidgetType::Scroller;
	ContentPanel = ScrollBox;
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CONSTRUCTOR %d"), 4));

	RefreshRecruitables();
	
}

void URecruitableCharactersDroppableWidget::DropFrom(UDraggedCharacterWidget* DraggedWidget)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DROPPPED FROM RECRUIT"));
}

bool URecruitableCharactersDroppableWidget::DropTo(UDraggedCharacterWidget* DraggedWidget)
{
   

	return false;
}

void URecruitableCharactersDroppableWidget::UpdateChildIndices()
{
	if(ScrollBox)
	{
		for (int i = 0; i < ScrollBox->GetChildrenCount(); ++i)
		{
			UDraggedCharacterWidget* Child = Cast<UDraggedCharacterWidget>(ScrollBox->GetChildAt(i));
			if(Child)
			{
				Child->SetPreviousChildIndex(i);
			}
		}
	}
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
			UDraggedCharacterWidget* NewWidget = CreateWidget<UDraggedCharacterWidget>(this->GetWorld(), AWidgetManager::GetWidget(EWidgetKeys::DraggedRecruitWidget, GetWorld()));
			if(NewWidget)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("REFRESH"));
				ScrollBox->AddChild(NewWidget);
				NewWidget->SetOwnerAreaWidget(this);
				// NewWidget->LatestChildIndex = ScrollBox->GetChildIndex(NewWidget);

				 // if(NewWidget->Stat == nullptr)
			  //   {
			        FCharacterStats* NewCharacter = ACharacterGenerationManager::CreateRandomCharacter(GetWorld());

			        if(NewCharacter)
			        {
			        	NewWidget->SetStat(NewCharacter);
			        	ImageManager::SetPortraitTextureByClass(NewCharacter->ClassType, NewWidget->Portrait);
			            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DRAGGABLE NAME %s"), *this->GetName()));
			        }

			    //}
			}
		}
	}
}
