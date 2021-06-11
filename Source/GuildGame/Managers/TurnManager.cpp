// Fill out your copyright notice in the Description page of Project Settings.


#include "GuildGame/Managers/TurnManager.h"

#include "GGLogHelper.h"
#include "GuildGame/Characters/GGCharacter.h"


void TurnManager::Start()
{
	if(CharactersList.Num() == 0)
	{
		return;
	}

	if(CharactersList[0] != nullptr)
	{
		CharactersList[0]->SetSelected();
	}
	CurrentCharacterIndex = 0;
}

void TurnManager::NextCharacter()
{
	if(CharactersList.Num() == 0)
	{
		return;
	}

	if(CharactersList[CurrentCharacterIndex] != nullptr)
	{
		CharactersList[CurrentCharacterIndex]->Deselect();
	}

	for(int i = 0; i < CharactersList.Num(); i++)
	{
		CurrentCharacterIndex+=1;
		if(CurrentCharacterIndex >= CharactersList.Num())
		{
			CurrentCharacterIndex = 0;
			TurnCount++;
		}

		if(CharactersList[CurrentCharacterIndex] != nullptr)
		{
			if(CharactersList[CurrentCharacterIndex]->GetStatus() != ECharacterStatus::Dead)
			{
				break;
			}
		}
	}

	if(CharactersList[CurrentCharacterIndex] != nullptr)
	{
		CharactersList[CurrentCharacterIndex]->SetSelected();
	}
	
	
}

void TurnManager::SortCharactersBySpeed()
{
	Algo::Sort(CharactersList, [](const AGGCharacter* LHS, const AGGCharacter* RHS)
	{
		return LHS->GetSpeed() > RHS-> GetSpeed();
	});
}
