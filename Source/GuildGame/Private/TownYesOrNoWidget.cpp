// Fill out your copyright notice in the Description page of Project Settings.


#include "TownYesOrNoWidget.h"


#include "TownGameModeBase.h"
#include "TownPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

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

void UTownYesOrNoWidget::CreateYesNoWidget(UUserWidget* Owner, UTownYesOrNoWidget* Widget, FText& Title, FText& Content,
    FYesNoDelegate YesDelegate, FYesNoDelegate NoDelegate)
{

    if(Widget == nullptr)
        return;
    
    ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(Owner->GetWorld()));
    ATownPlayerController* PlayerController = Cast<ATownPlayerController>(UGameplayStatics::GetPlayerController(Owner->GetWorld(), 0));

    if(GameMode == nullptr)
        return;

   if(GameMode->YesOrNoWidgetInstance == nullptr)
    {

        UE_LOG(LogTemp, Warning, TEXT("CREATE is called"));
        UTownYesOrNoWidget* NewWidget = CreateWidget<UTownYesOrNoWidget>(PlayerController, Widget->GetClass());
	    NewWidget->AddToViewport(9999);
        GameMode->YesOrNoWidgetInstance = NewWidget;
    }
    else
    {
        GameMode->YesOrNoWidgetInstance->RemoveFromViewport();
        GameMode->YesOrNoWidgetInstance->AddToViewport(9999);
    }

   /* FInputModeGameAndUI Mode;
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PlayerController->SetInputMode(Mode);*/
    
    GameMode->YesOrNoWidgetInstance->NoEvent = NoDelegate;
    GameMode->YesOrNoWidgetInstance->YesEvent = YesDelegate;
    if(GameMode->YesOrNoWidgetInstance->TitleTextBlock)
        GameMode->YesOrNoWidgetInstance->TitleTextBlock->SetText(Title);

    if(GameMode->YesOrNoWidgetInstance->ContentTextBlock)
        GameMode->YesOrNoWidgetInstance->ContentTextBlock->SetText(Content);
    
    GameMode->YesOrNoWidgetInstance->SetVisibility(ESlateVisibility::Visible);
    
}
