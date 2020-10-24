// Fill out your copyright notice in the Description page of Project Settings.


#include "TownMenuWidget.h"



#include "TownBuildingWidgetBase.h"
#include "TownDefaultPawn.h"
#include "TownGameInstance.h"
#include "TownYesOrNoWidget.h"
#include "Components/Button.h"
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

     if(RecruitGoldTxt)
        RecruitGoldTxt->SetText(FText::AsNumber(RecruitBuildingPrice));

     if(BuildRecruitBtn)
        BuildRecruitBtn->OnClicked.AddUniqueDynamic(this, &UTownMenuWidget::OnBuildRecruit);
    
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
         UGameplayStatics::OpenLevel(GetWorld(), "TestMap");
     // }
}

void UTownMenuWidget::OnBuildRecruit()
{
      ATownDefaultPawn* Pawn = Cast<ATownDefaultPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
    if(Pawn)
    {
        /*FYesNoDelegate NoEvent;
        NoEvent.BindDynamic(this, &UTownBuildingWidgetBase::ConstrNoEvent);
        FYesNoDelegate YesEvent;
        YesEvent.BindDynamic(this, &UTownBuildingWidgetBase::ConstrYesEvent);
                
        UTownYesOrNoWidget::CreateYesNoWidget(this
            , Pawn->YesOrNoWidgetBP ,YesEvent, NoEvent);*/
    }
}

void UTownMenuWidget::UpdateUI()
{
    UTownGameInstance* GameInstance = Cast<UTownGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(GoldText && GameInstance)
        GoldText->SetText(FText::AsNumber(GameInstance->Gold));
}
