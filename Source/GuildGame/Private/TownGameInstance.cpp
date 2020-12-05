// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameInstance.h"

#include "TownGameModeBase.h"
#include "WidgetManager.h"

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
