// Fill out your copyright notice in the Description page of Project Settings.


#include "GuildGame/Town/TownGameInstance.h"
#include "TownGameModeBase.h"
#include "GuildGame/Managers/WidgetManager.h"

bool UTownGameInstance::TryToPurchase(int Amount)
{
    if(Gold >= Amount)
    {
        Gold -= Amount;
        ATownGameModeBase::UpdateUI(this);//TODO
        return true;
    }
    else
    {
        return false;
    }
}

void UTownGameInstance::IncreaseDay(int Amount)
{
    Day += Amount;
}


void UTownGameInstance::Init()
{
    if(WidgetsTable)
    {
        WidgetManager::SetWidgetTable(WidgetsTable);
    }

    if(SequenceTable)
    {
        WidgetManager::SetSequenceTable(SequenceTable);
    }

    
}
