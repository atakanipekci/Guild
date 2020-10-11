// Fill out your copyright notice in the Description page of Project Settings.


#include "TownTavernWidget.h"

#include "TownDefaultPawn.h"
#include "Kismet/GameplayStatics.h"

void UTownTavernWidget::NativeConstruct()
{
    Super::NativeConstruct();
    OnEnabled();
}

void UTownTavernWidget::OnEnabled()
{
    Super::OnEnabled();

    // ATownDefaultPawn* Pawn = Cast<ATownDefaultPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
    // if(Pawn)
    // {
    //     
    // }
}


