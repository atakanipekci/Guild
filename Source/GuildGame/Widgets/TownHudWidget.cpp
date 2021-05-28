// Fill out your copyright notice in the Description page of Project Settings.


#include "TownHudWidget.h"
#include "BuildingWidgetBase.h"
#include "GuildDetailsWidget.h"
#include "GuildGameInstance.h"
#include "GuildGame/Town/TownBuildingInteractionManager.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "Kismet/GameplayStatics.h"

void UTownHudWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    if(TestButton)
        TestButton->OnClicked.AddUniqueDynamic(this, &UTownHudWidget::OnTestClicked);

    if(GuildButton)
        GuildButton->OnClicked.AddUniqueDynamic(this, &UTownHudWidget::OpenGuildDetailsScreen);

    if(TestTextBlock)
        TestTextBlock->SetText(FText::FromString("Load Next Level"));

    UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(GetGameInstance());
    
    if(GoldText && GameInstance)
        GoldText->SetText(FText::AsNumber(GameInstance->Gold));

    PlayerController = Cast<ATownPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

}



void UTownHudWidget::OnTestClicked()
{
    // UE_LOG(LogTemp, Warning, TEXT("I just started running"));
    if(TestTextBlock)
        TestTextBlock->SetText(FText::FromString("Level Loaded"));

     UWorld* TheWorld = GetWorld();
 
     FString CurrentLevel = TheWorld->GetMapName();
     UGameplayStatics::OpenLevel(GetWorld(), "TestMap");

    //TODO save towngamemode data and delete all pointers 
    
     // if (CurrentLevel == "ThirdPersonExampleMap")
     // {
     //     UGameplayStatics::OpenLevel(GetWorld(), "Level2");
     // }
     // else
     // {
        //  UGameplayStatics::OpenLevel(GetWorld(), "TownBase");
        //
        // UTownGameInstance* GameInstance = Cast<UTownGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        // if(GameInstance)
        // {
        //     GameInstance->IncreaseDay(1);
        // }
    
     // }
}

void UTownHudWidget::OpenGuildDetailsScreen()
{
    UUserWidget* NewWidget = WidgetManager::GetOrCreateWidgetInstance(EWidgetKeys::GuildDetail, this);
    if(NewWidget)
    {
        //NewWidget->RemoveFromViewport();
        NewWidget->SetVisibility(ESlateVisibility::Visible);
        if(NewWidget->IsInViewport() == false)
        {
            NewWidget->AddToViewport();
        }
    }

    UGuildDetailsWidget* GuildDetail =  Cast<UGuildDetailsWidget>(NewWidget);
    if(GuildDetail)
    {
        UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(GetGameInstance());
        if(GameInstance)
        {
            GuildDetail->RefreshPage(GameInstance->GuildStats);
        }
    }
}

void UTownHudWidget::ExitCanvas(EMenuWidgetType CanvasType)
{
     if(PlayerController)
    {
        if(PlayerController->BuildingInteractionManager)
            PlayerController->BuildingInteractionManager->ToPrevStateState();
    }
    if(CanvasType == EMenuWidgetType::TeamCanvas)
    {
        TeamCanvas->SetVisibility(ESlateVisibility::Collapsed);
    }
    else if(CanvasType == EMenuWidgetType::WarCanvas)
    {
        WarCanvas->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UTownHudWidget::EnterCanvas(EMenuWidgetType CanvasType)
{
    if(PlayerController)
    {
        if(PlayerController->BuildingInteractionManager)
            PlayerController->BuildingInteractionManager->ChangeState(PlayerController->BuildingInteractionManager->State, EInteractionStatee::MenuWidget);
    }
    if(CanvasType == EMenuWidgetType::TeamCanvas)
    {
        TeamCanvas->SetVisibility(ESlateVisibility::Visible);
    }
    else if(CanvasType == EMenuWidgetType::WarCanvas)
    {
        WarCanvas->SetVisibility(ESlateVisibility::Visible);
    }
}


void UTownHudWidget::UpdateUI()
{
    UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if(GameInstance)
    {
        if(GoldText)
            GoldText->SetText(FText::AsNumber(GameInstance->Gold));

        if(DayText)
        {
            const int Day = GameInstance->Day;
            
            const FText CostLocalizedText = NSLOCTEXT("CommonWords", "Day:", "DAY {Day}");
		    FFormatNamedArguments Args;
		    Args.Add("Day", Day);
		    
		    const FText FormattedText = FText::Format(
		    	CostLocalizedText,
		    	Args
		    );
            DayText->SetText(FormattedText);
            
            // FString DayString = "DAY ";
            // DayString.AppendInt(GameInstance->Day);
            // DayText->SetText(FText::FromString(DayString));
        }
        
    }
}
