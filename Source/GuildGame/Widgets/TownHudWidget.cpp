// Fill out your copyright notice in the Description page of Project Settings.


#include "TownHudWidget.h"
#include "BuildingWidgetBase.h"
#include "GuildGameInstance.h"
#include "GuildGame/Town/TownInteractionController.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UTownHudWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    if(TestButton)
        TestButton->OnClicked.AddUniqueDynamic(this, &UTownHudWidget::OnTestClicked);

    if(TestTextBlock)
        TestTextBlock->SetText(FText::FromString("Load Next Level"));

    UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
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

void UTownHudWidget::ExitCanvas(EMenuWidgetType CanvasType)
{
     if(PlayerController)
    {
        if(PlayerController->InteractionController)
            PlayerController->InteractionController->ToPrevStateState();
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
        if(PlayerController->InteractionController)
            PlayerController->InteractionController->ChangeState(PlayerController->InteractionController->State, EInteractionStatee::MenuWidget);
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
    
    if(GoldText && GameInstance)
        GoldText->SetText(FText::AsNumber(GameInstance->Gold));
}
