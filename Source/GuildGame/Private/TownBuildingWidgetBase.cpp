// Fill out your copyright notice in the Description page of Project Settings.


#include "TownBuildingWidgetBase.h"


#include "TownBuildingActorComponent.h"
#include "TownDefaultPawn.h"
#include "TownGameInstance.h"
#include "TownGameModeBase.h"
#include "TownInteractionController.h"
#include "TownPlayerController.h"
#include "TownYesOrNoWidget.h"
#include "WidgetManager.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"

void UTownBuildingWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

     if(ConstructButton)
        ConstructButton->OnClicked.AddUniqueDynamic(this, &UTownBuildingWidgetBase::OnConstructClicked);

     PlayerController = Cast<ATownPlayerController>(UGameplayStatics::GetPlayerController(this, 0));


    OnEnabled();
}

void UTownBuildingWidgetBase::OnEnabled()
{
    if(PlayerController)
    {
        if(PlayerController->InteractionController)
        {
            if(PlayerController->InteractionController->SelectedBuilding)
            {
                if(PlayerController->InteractionController->SelectedBuilding->GetConstructionState() == EBuildingConstructionState::Constructed)
                {
                    if(MainCanvasPanel)
                        MainCanvasPanel->SetVisibility(ESlateVisibility::Visible);
                }
                else if(PlayerController->InteractionController->SelectedBuilding->GetConstructionState() == EBuildingConstructionState::NotConstructed)
                {
                    if(MainCanvasPanel)
                        MainCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
                    //this->SetVisibility(ESlateVisibility::Collapsed);
                    if(PreviewCanvasPanel)
                        PreviewCanvasPanel->SetVisibility(ESlateVisibility::Visible);

                    PlayerController->InteractionController->SelectedBuilding->SetConstructionState(EBuildingConstructionState::Preview);
                }
                else if(PlayerController->InteractionController->SelectedBuilding->GetConstructionState() == EBuildingConstructionState::UnderConstruction)
                {
                    if(MainCanvasPanel)
                        MainCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
                    //this->SetVisibility(ESlateVisibility::Collapsed);
                    if(PreviewCanvasPanel)
                        PreviewCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);

                    if(UnderConstructionCanvasPanel)
                        UnderConstructionCanvasPanel->SetVisibility(ESlateVisibility::Visible);
                }
            }
            
        }
        
    }
}

void UTownBuildingWidgetBase::OnExitClicked()
{
    if(PlayerController)
    {
        if(PlayerController->InteractionController)
            PlayerController->InteractionController->ZoomOutFromBuilding();
    }
}

void UTownBuildingWidgetBase::OnConstructClicked()
{
    FYesNoDelegate NoEvent;
    NoEvent.BindDynamic(this, &UTownBuildingWidgetBase::ConstrNoEvent);
    FYesNoDelegate YesEvent;
    YesEvent.BindDynamic(this, &UTownBuildingWidgetBase::ConstrYesEvent);

    FText Title = FText::FromString("Construct Building");
    FText Content = FText::FromString("Are you sure about purchasing this building?");

    UTownYesOrNoWidget::CreateYesNoWidget(this, WidgetManager::GetWidget(EWidgetKeys::YesOrNo), Title, Content ,YesEvent, NoEvent);
}

bool UTownBuildingWidgetBase::ConstrNoEvent()
{
   /* ATownDefaultPawn* Pawn = Cast<ATownDefaultPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
    if(Pawn)
    {
        Pawn->ZoomOutFromBuilding();
    }*/
    return  true;
}

bool UTownBuildingWidgetBase::ConstrYesEvent()
{


    ATownGameModeBase* Mode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this));
    UTownGameInstance* Instance = Cast<UTownGameInstance>(UGameplayStatics::GetGameInstance(this));
    if(PlayerController && Mode && Instance)
    {
        if(PlayerController->InteractionController)
        {
            if(PlayerController->InteractionController->SelectedBuilding)
            {
                if(Mode->BuildingDataTable)
                {
                    const FString Context(TEXT("CONTEXT DATATABLE TEXT"));
                    const FName Row = *(PlayerController->InteractionController->SelectedBuilding->BuildingDataTableKey);
                    FBuildingDataTable* BuildingData = Mode->BuildingDataTable->FindRow<FBuildingDataTable>(Row, Context, true);

                    if(BuildingData)
                    {
                        const bool Response = Instance->TryToPurchase(BuildingData->ConstructionPrice);
                        if(Response == true)
                        {
                            PlayerController->InteractionController->SelectedBuilding->SetConstructionState(EBuildingConstructionState::UnderConstruction);
                            /*Pawn->ZoomOutFromBuilding();*/
                            OnEnabled();
                            return  true;
                        }
                        else
                        {
                            return  false;
                        }
                    }
                }
            }
        }
    }
    return  false;

    
}
