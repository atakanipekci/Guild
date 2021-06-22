// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */
class GUILDGAME_API TurnManager
{
public:

	void Start();
	
	void NextCharacter();
	
	void SetGameMode(class AGuildGameGameModeBase* Mode)
	{
		GameMode = Mode;
	}

	void SetCharactersList(TArray<class AGGCharacter*> List)
	{
		CharactersList = List;
		SortCharactersBySpeed();
	}

	AGGCharacter* GetCurrentCharacter() const
	{
		if(CharactersList.Num() == 0)
		{
			return nullptr;
		}
		return CharactersList[CurrentCharacterIndex];
	}
 
private:

	void SortCharactersBySpeed();
	
    class AGuildGameGameModeBase* GameMode = nullptr;
	int CurrentCharacterIndex = 0;
	int RoundCount = 0;
	TArray<class AGGCharacter*> CharactersList;
};
