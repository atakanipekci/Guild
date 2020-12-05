// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameModeBase.h"


#include "TownMenuWidget.h"
#include "TownPlayerController.h"
#include "Kismet/GameplayStatics.h"

ATownGameModeBase::ATownGameModeBase()
{
    PlayerControllerClass = ATownPlayerController::StaticClass();
}

void ATownGameModeBase::UpdateUI(UObject* Caller)
{
    ATownGameModeBase* Mode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(Caller));

    if(Mode)
    {
        if(Mode->MenuWidget)
        {
            Mode->MenuWidget->UpdateUI();
        }
    }
}


