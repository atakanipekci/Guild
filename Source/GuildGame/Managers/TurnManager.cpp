// Fill out your copyright notice in the Description page of Project Settings.


#include "GuildGame/Managers/TurnManager.h"

#include "CharacterManager.h"
#include "GGLogHelper.h"
#include "GridManager.h"
#include "Algo/StableSort.h"
#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/Widgets/TurnInfoWidget.h"


void TurnManager::Start()
{
	if(CharactersList.Num() == 0)
	{
		return;
	}

	GridMan = CharacterManager::CharGridManager;


	if(CharactersList[0] != nullptr)
	{
		CharactersList[0]->SetSelected();
	}
	CurrentCharacterIndex = 0;

	StartTurnWidget();
}

void TurnManager::NextCharacter()
{
	if(CharactersList.Num() == 0 || CurrentCharacterIndex >= CharactersList.Num())
	{
		return;
	}

	if(CharactersList[CurrentCharacterIndex] != nullptr)
	{
		if(GridMan)
		{
			GridMan->ApplyGridStatusEffectsToCharacter(CharactersList[CurrentCharacterIndex], CharactersList[CurrentCharacterIndex]->GetCurrentIndex());
		}
		CharactersList[CurrentCharacterIndex]->OnTurnEnds();
		CharactersList[CurrentCharacterIndex]->Deselect();

		UpdateWidgetOrder(true);//after status effects end update widget
	}

	for(int i = 0; i < CharactersList.Num(); i++)
	{
		CurrentCharacterIndex += 1;
		if(CurrentCharacterIndex >= CharactersList.Num())
		{
			if(TurnInfoWidget)
			{
				TurnInfoWidget->RefreshIndices();
				SortCharactersBySpeed(CharactersList);
				UpdateWidgetOrder();
			}

			
			if(GridMan)
			{
				GridMan->DecreaseGridStatusEffectTurns();
			}
			
			CurrentCharacterIndex = 0;
			
			
			RoundCount++;
			if(GameMode)
			{
				GameMode->OnRoundEnds();
			}
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
		if(TurnInfoWidget)
		{
			TurnInfoWidget->RemoveAtTop(true);
		}

		CharactersList[CurrentCharacterIndex]->SetSelected();
		// CharactersList[CurrentCharacterIndex]->OnTurnBegins();
	}
}

void TurnManager::SetCharactersList(TArray<AGGCharacter*> List)
{
	CharactersList = List;
	SortCharactersBySpeed(CharactersList);
}

void TurnManager::SortCharactersBySpeed(TArray<class AGGCharacter*>& ArrayToShuffle, int StartIndex)
{
	if(StartIndex < 0)
	{
		Algo::StableSort(ArrayToShuffle, [](const AGGCharacter* LHS, const AGGCharacter* RHS)
		{
			if(LHS && RHS)
			{
				return LHS->GetSpeed() > RHS-> GetSpeed();
			}
			return false;
		});
	}
	else if(StartIndex < ArrayToShuffle.Num())
	{
		TArray<class AGGCharacter*> CopyShuffleArray(ArrayToShuffle);

		for (int i = StartIndex - 1; i >= 0; --i)
		{
			CopyShuffleArray.RemoveAt(i);
		}

		Algo::StableSort(CopyShuffleArray, [](const AGGCharacter* LHS, const AGGCharacter* RHS)
		{
			if(LHS && RHS)
			{
				return LHS->GetSpeed() > RHS-> GetSpeed();
			}
			return false;
		});

		int j = 0;
		for (int i = StartIndex; i < ArrayToShuffle.Num(); ++i)
		{
			ArrayToShuffle[i] = CopyShuffleArray[j];
			j++;
		}
	}
	
}

void TurnManager::UpdateOnDeath(AGGCharacter* DeadChar)
{
	if(TurnInfoWidget)
	{
		TurnInfoWidget->RemoveDeadCharacter(DeadChar);
	}
}

void TurnManager::UpdateWidgetOrder(bool SortCharList)
{
	if(TurnInfoWidget)
	{
		if(SortCharList)
		{
			SortCharactersBySpeed(CharactersList, CurrentCharacterIndex + 1);
		}
		TurnInfoWidget->SetCharactersList(CharactersList);
		TurnInfoWidget->UpdateOrder();
	}
}

void TurnManager::StartTurnWidget()
{
	if(TurnInfoWidget && CharactersList.Num() > 0)
	{
		TurnInfoWidget->Start(CharactersList);
	}
}


