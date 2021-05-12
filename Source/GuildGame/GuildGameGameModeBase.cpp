// Copyright Epic Games, Inc. All Rights Reserved.


#include "GuildGameGameModeBase.h"


#include "GuildGameInstance.h"
#include "Characters/CharacterStats.h"
#include "Characters/GGCharacter.h"
#include "Managers/CharacterManager.h"
#include "Kismet/GameplayStatics.h"

void AGuildGameGameModeBase::BeginPlay()
{
	 UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
     if(GameInstance)
     {
     	float pos_x = 0;
     	for (auto Element : GameInstance->SquadCharacters)
     	{
			CharacterManager::SpawnCharacter<AGGCharacter,AGGCharacter>(BattleCharactersBP,Element->ClassType,
           									FVector{pos_x,0,110}, FRotator{}, GetWorld());
     		
     		pos_x += 100;
     	}   
     }
}
