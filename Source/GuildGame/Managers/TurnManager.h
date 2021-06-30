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

	void SetCharactersList(TArray<class AGGCharacter*> List);
	

	void SetTurnWidget(class UTurnInfoWidget* Widget)
	{
		TurnInfoWidget = Widget;
	}

	AGGCharacter* GetCurrentCharacter() const
	{
		if(CharactersList.Num() == 0)
		{
			return nullptr;
		}
		return CharactersList[CurrentCharacterIndex];
	}

	void UpdateOnDeath(class AGGCharacter* DeadChar);
	void UpdateWidgetOrder(bool SortCharList = false);
	
private:

	void SortCharactersBySpeed(TArray<class AGGCharacter*>& ArrayToShuffle, int StartIndex = INDEX_NONE);
	void StartTurnWidget();
	
    class AGuildGameGameModeBase* GameMode = nullptr;
	int CurrentCharacterIndex = 0;
	int RoundCount = 0;
	TArray<class AGGCharacter*> CharactersList;

	class UTurnInfoWidget* TurnInfoWidget = nullptr;
};
