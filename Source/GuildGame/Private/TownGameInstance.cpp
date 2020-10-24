// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameInstance.h"

#include "TownGameModeBase.h"

bool UTownGameInstance::TryToPurchase(int Amount)
{
    if(Gold >= Amount)
    {
        Gold -= Amount;
        UE_LOG(LogTemp, Warning, TEXT("Yes %d"), Gold);
        ATownGameModeBase::UpdateUI(this);
        return true;
    }
    else
    {
        return false;
    }
}
