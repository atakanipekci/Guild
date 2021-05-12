// Fill out your copyright notice in the Description page of Project Settings.


#include "GuildGameInstance.h"

#include "GuildGame/GuildStats.h"
#include "GuildGame/Managers/CharacterGenerationManager.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "Kismet/GameplayStatics.h"

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

void UGuildGameInstance::ReturnToTown()
{
    UGameplayStatics::OpenLevel(GetWorld(), "TownBase");
    Day++;
}

void UGuildGameInstance::IncreaseDay(int Amount)
{
    Day += Amount;
}


void UGuildGameInstance::Init()
{
    Day = 0;
    if(WidgetsTable)
    {
        WidgetManager::SetWidgetTable(WidgetsTable);
    }

    if(SequenceTable)
    {
        WidgetManager::SetSequenceTable(SequenceTable);
    }

     if(CharacterStatsTable)
    {
        CharacterGenerationManager::SetCharactersTable(CharacterStatsTable);
    }

    GuildStats = new FGuildStats();
    
    // FGuildStats* GuildStats = new FGuildStats();
    // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("FAME LEVEL  %d"), GuildStats->FameLevel));
    // // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Squad count  %s"), *FameLevelsDisplayNames[0]));
    //
    // GuildStats->GainFameExperience(1500);
    // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("FAME LEVEL  %d"), GuildStats->FameLevel));
    // // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Squad count  %s"), *FameLevelsDisplayNames[0]));
}
