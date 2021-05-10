// Fill out your copyright notice in the Description page of Project Settings.


#include "RecruitWidget.h"
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