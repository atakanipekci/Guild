// Fill out your copyright notice in the Description page of Project Settings.


#include "TownRecruitWidget.h"



#include "TownBuildingActorComponent.h"
#include "TownDefaultPawn.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"

class ATownDefaultPawn;

void UTownRecruitWidget::NativeConstruct()
{
    Super::NativeConstruct();
    OnEnabled();
}

void UTownRecruitWidget::OnEnabled()
{
    Super::OnEnabled();

    ATownDefaultPawn* Pawn = Cast<ATownDefaultPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
    if(Pawn && ConfirmBorder && MainBorder)
    {
        if(Pawn->SelectedBuilding)
        {
            if(Pawn->SelectedBuilding->IsConstructed() == true)
            {
                UE_LOG(LogTemp, Warning, TEXT("WIDGET TRUE"));
                ConfirmBorder->SetVisibility(ESlateVisibility::Collapsed);
                MainBorder->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("WIDGET FALSE"));
                ConfirmBorder->SetVisibility(ESlateVisibility::Visible);
                MainBorder->SetVisibility(ESlateVisibility::Collapsed);
            }
        }
        
    }
}