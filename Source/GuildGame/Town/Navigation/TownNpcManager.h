// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GuildGame/Characters/CharacterStats.h"
#include "UObject/NoExportTypes.h"
// #include "TownNpcManager.generated.h"

/**
 * 
 */


class GUILDGAME_API TownNpcManager
{

	public:
	TownNpcManager(class ATownGameModeBase* GameMode);

	void StartSpawning(TArray<struct FCharacterStats*> OwnedCharacters);

	void SpawnOnClick();
	void SpawnNpcToRandomLocation(int UniqueID, ENpcBehaviourStates State, ECharacterClassType CharacterType);
	//class ATownNpcCharacter* SpawnCharacter(ECharacterClassType CharacterType, ENpcBehaviourStates State);
	FString GetNpcTableRowName(ECharacterClassType CharacterType);

	class ATownNavNodeActor* GetRandomNavigationNode();
	FVector GetRandomLocationFromNode(float Zoffset);
	void MoveNpcToDestination(class ATownNpcCharacter* SpawnedActor);

	void SetNpcBehaviourState(int UniqueID, const ENpcBehaviourStates State, ECharacterClassType CharacterType);

	ATownGameModeBase* TownGameMode;
	TArray<class ATownNavNodeActor*> NavigationNodes;
	TMap<int, class ATownNpcCharacter*> SpawnedNpCs;

	

};



