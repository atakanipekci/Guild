// Fill out your copyright notice in the Description page of Project Settings.


#include "RecruitableCharactersWidget.h"


#include "DraggedCharacterWidget.h"
#include "WidgetManager.h"
#include "Components/ScrollBox.h"

void URecruitableCharactersWidget::NativeConstruct()
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
			}
		}
	}
	
}

void URecruitableCharactersWidget::DropFrom(UDraggedCharacterWidget* DraggedWidget)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DROPPPED FROM RECRUIT"));
}

bool URecruitableCharactersWidget::DropTo(UDraggedCharacterWidget* DraggedWidget)
{
   

	return false;
}