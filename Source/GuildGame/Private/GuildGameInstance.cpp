// Fill out your copyright notice in the Description page of Project Settings.


#include "GuildGameInstance.h"

#include "GuildGame/Managers/CharacterGenerationManager.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "GuildGame/Managers/WidgetManager.h"

bool UGuildGameInstance::TryToPurchase(int Amount)
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

void UGuildGameInstance::IncreaseDay(int Amount)
{
    Day += Amount;
}


void UGuildGameInstance::Init()
{
    if(WidgetsTable)
    {
        WidgetManager::SetWidgetTable(WidgetsTable);
    }

    if(SequenceTable)
    {
        WidgetManager::SetSequenceTable(SequenceTable);
    }

     if(CharactersTable)
    {
        CharacterGenerationManager::SetCharactersTable(CharactersTable);
    }

    
}
