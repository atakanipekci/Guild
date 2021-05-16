// Copyright Epic Games, Inc. All Rights Reserved.


#include "GuildGameGameModeBase.h"


#include "GuildGameInstance.h"
#include "Characters/CharacterStats.h"
#include "Characters/GGCharacter.h"
#include "Managers/CharacterManager.h"
#include "Managers/GridManager.h"
#include "GGLogHelper.h"
#include "Kismet/GameplayStatics.h"

void AGuildGameGameModeBase::BeginPlay()
{
	 UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
     if(GameInstance)
     {
     	int Count = 0;
     	GridManager* GridMan = CharacterManager::CharGridManager;
     	for (auto Element : GameInstance->SquadCharacters)
     	{
     		if(GridMan)
     		{
     			FVector Location = GridMan->GetGridCenter(Count);
     			Location.Z = 100;
				AGGCharacter* Char = CharacterManager::SpawnCharacter<AGGCharacter,AGGCharacter>(BattleCharactersBP,Element->ClassType,
           										Location, FRotator(), GetWorld());

     			if(Char)
     			{
     				Char->SetStats(*Element);
     				Char->SetCurrentIndex(Count);
					GridMan->SetGridState(Count, EGridState::Obstacle);
     			}
     		}
     		Count+=2;
     	}   
     }
}
