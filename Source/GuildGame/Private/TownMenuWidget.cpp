// Fill out your copyright notice in the Description page of Project Settings.


#include "TownMenuWidget.h"



#include "TownBuildingWidgetBase.h"
#include "TownGameInstance.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UTownMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    if(TestButton)
        TestButton->OnClicked.AddUniqueDynamic(this, &UTownMenuWidget::OnTestClicked);

    if(TestTextBlock)
        TestTextBlock->SetText(FText::FromString("Load Next Level"));

    UTownGameInstance* GameInstance = Cast<UTownGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(GoldText && GameInstance)
        GoldText->SetText(FText::AsNumber(GameInstance->Gold));

     

    
    
}



void UTownMenuWidget::OnTestClicked()
{
    // UE_LOG(LogTemp, Warning, TEXT("I just started running"));
    if(TestTextBlock)
        TestTextBlock->SetText(FText::FromString("Level Loaded"));

     UWorld* TheWorld = GetWorld();
 
     FString CurrentLevel = TheWorld->GetMapName();
     
     // if (CurrentLevel == "ThirdPersonExampleMap")
     // {
     //     UGameplayStatics::OpenLevel(GetWorld(), "Level2");
     // }
     // else
     // {
         //UGameplayStatics::OpenLevel(GetWorld(), "TestMap");
     // }
}


void UTownMenuWidget::UpdateUI()
{
    UTownGameInstance* GameInstance = Cast<UTownGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(GoldText && GameInstance)
        GoldText->SetText(FText::AsNumber(GameInstance->Gold));
}
