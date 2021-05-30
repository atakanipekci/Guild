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
           										Location, FRotator::ZeroRotator, GetWorld());


     			FCharFileDataTable* CharFiles = nullptr;
     			if(Element)
     			{
     				const FName RowName = *(CharacterManager::GetCharFileRowName(Element->ClassType));
					CharFiles = GameInstance->CharacterFileTable->FindRow<FCharFileDataTable>(RowName, "Character File Row Missing", true);
     			}

     			if(Char)
     			{
     				if(Element)
     					Char->SetStats(*Element);

     				if(CharFiles)
     					Char->SetFile(*CharFiles);

     				Char->PrepareAnimInstance();

     				Char->SetCurrentIndex(Count);
					GridMan->SetGridState(Count, EGridState::Obstacle);
     				Characters.Add(Char);
     			}
     		}
     		Count+=2;
     	}   
     }
}
