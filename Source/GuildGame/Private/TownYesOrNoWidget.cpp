// Fill out your copyright notice in the Description page of Project Settings.


#include "TownYesOrNoWidget.h"


#include "TownGameModeBase.h"
#include "TownPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

UTownYesOrNoWidget* UTownYesOrNoWidget::YesOrNoWidgetInstance = nullptr;

void UTownYesOrNoWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if(YesButton)
        YesButton->OnClicked.AddUniqueDynamic(this, &UTownYesOrNoWidget::OnYesButtonClicked);
     if(NoButton)
        NoButton->OnClicked.AddUniqueDynamic(this, &UTownYesOrNoWidget::OnNoButtonClicked);

    if(TitleTextBlock)
        TitleTextBlock->SetText(FText::FromString("Title"));

    if(ContentTextBlock)
        ContentTextBlock->SetText(FText::FromString("Context"));
}

void UTownYesOrNoWidget::OnYesButtonClicked()
{
    if(YesEvent.IsBound())
   {
       const bool Response = YesEvent.Execute();
        if(Response == true)
        {
            this->SetVisibility(ESlateVisibility::Collapsed);
        }
   }
    else
    {
        this->SetVisibility(ESlateVisibility::Collapsed);
    }
    //this->RemoveFromParent();
}

void UTownYesOrNoWidget::OnNoButtonClicked()
{
   if(NoEvent.IsBound())
   {
       NoEvent.Execute();
   }
   //this->RemoveFromParent();
    this->SetVisibility(ESlateVisibility::Collapsed);
}

void UTownYesOrNoWidget::CreateYesNoWidget(UUserWidget* Owner, TSubclassOf<UUserWidget> Widget, FText& Title, FText& Content,
    FYesNoDelegate YesDelegate, FYesNoDelegate NoDelegate)
{
    if(Widget == nullptr)
        return;
    
   if(YesOrNoWidgetInstance == nullptr)
    {
        UTownYesOrNoWidget* NewWidget = CreateWidget<UTownYesOrNoWidget>(Owner->GetWorld(), Widget);
       if(NewWidget)
       {
	        NewWidget->AddToViewport(9999);
            YesOrNoWidgetInstance = NewWidget;
       }
    }
    else
    {
        YesOrNoWidgetInstance->RemoveFromViewport();
        YesOrNoWidgetInstance->AddToViewport(9999);
    }

    if(YesOrNoWidgetInstance)
    {
        YesOrNoWidgetInstance->NoEvent = NoDelegate;
        YesOrNoWidgetInstance->YesEvent = YesDelegate;

        if(YesOrNoWidgetInstance->TitleTextBlock)
           YesOrNoWidgetInstance->TitleTextBlock->SetText(Title);

        if(YesOrNoWidgetInstance->ContentTextBlock)
           YesOrNoWidgetInstance->ContentTextBlock->SetText(Content);
        
        YesOrNoWidgetInstance->SetVisibility(ESlateVisibility::Visible);
    }
    
}
