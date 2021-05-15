// Fill out your copyright notice in the Description page of Project Settings.


#include "RecruitWidget.h"

#include "BuildingUpgradeWidgetBase.h"
#include "RecruitableCharactersDroppableWidget.h"
#include "Components/Button.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "GuildGame/Town/BuildingStats.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "Kismet/GameplayStatics.h"
// #include "GuildGame/Town/TownBuildingActorComponent.h"
// #include "TownDefaultPawn.h"
// #include "Components/Border.h"
// #include "Kismet/GameplayStatics.h"
// #include "Misc/MessageDialog.h"

class ATownDefaultPawn;

void URecruitWidget::NativeConstruct()
{
    Super::NativeConstruct();
    //OnEnabled();
    if(RefreshButton)
        RefreshButton->OnClicked.AddUniqueDynamic(this, &URecruitWidget::RefreshRecruitables);
    
    if(UpgradeButton)
        UpgradeButton->OnClicked.AddUniqueDynamic(this, &URecruitWidget::OpenUpgradePage);
}

void URecruitWidget::Refresh()
{
    Super::Refresh();

    // ATownDefaultPawn* Pawn = Cast<ATownDefaultPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
    // if(Pawn && ConfirmBorder && MainBorder)
    // {
    //     if(Pawn->SelectedBuilding)
    //     {
    //         if(Pawn->SelectedBuilding->IsConstructed() == true)
    //         {
    //             ConfirmBorder->SetVisibility(ESlateVisibility::Collapsed);
    //             MainBorder->SetVisibility(ESlateVisibility::Visible);
    //         }
    //         else
    //         {
    //             //FText title = FText::FromString("My TITle");
    //             //FMessageDialog::Open(EAppMsgType::YesNo,FText::FromString("EEEY"),&title);
    //             ConfirmBorder->SetVisibility(ESlateVisibility::Visible);
    //             MainBorder->SetVisibility(ESlateVisibility::Collapsed);
    //         }
    //     }
    //     
    // }
}

void URecruitWidget::RefreshRecruitables()
{
    if(RecruitableCharactersSlider)
    {
        RecruitableCharactersSlider->RefreshRecruitables();
    }
}

void URecruitWidget::OpenUpgradePage()
{
    UUserWidget* UpgradePage = WidgetManager::GetOrCreateWidgetInstance(EWidgetKeys::RecruitUpgradeWidget, this);
    if(UpgradePage)
    {
        UpgradePage->RemoveFromViewport();
        UpgradePage->SetVisibility(ESlateVisibility::Visible);
        UpgradePage->AddToViewport();

        ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this));
        if(GameMode)
        {
            FBuildingStatsBase* BuildingStats = GameMode->BuildingStatsMap[EBuildingTypes::Recruit];
            UBuildingUpgradeWidgetBase* RecruitUpgradePage = Cast<UBuildingUpgradeWidgetBase>(UpgradePage);
            RecruitUpgradePage->RefreshPage(BuildingStats);
            
        }
    }
}
