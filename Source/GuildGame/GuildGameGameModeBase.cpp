// Copyright Epic Games, Inc. All Rights Reserved.


#include "GuildGameGameModeBase.h"


#include "GuildGameInstance.h"
#include "Characters/CharacterStats.h"
#include "Characters/GGCharacter.h"
#include "Managers/CharacterManager.h"
#include "Managers/GridManager.h"
#include "GGLogHelper.h"
#include "Battle/BattlePlayerController.h"
#include "GridSystem/GridFloor.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/WidgetManager.h"
#include "Widgets/BattleHudWidget.h"

void AGuildGameGameModeBase::BeginPlay()
{
	 UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
     if(GameInstance)
     {
     	int Count = 0;
     	GridManager* GridMan = CharacterManager::CharGridManager;
     	for (auto Element : GameInstance->SquadCharacters)
     	{
     		if(GridMan && GridMan->GetAttachedFloor())
     		{
     			TArray<Grid*> Placeables = (GridMan->GetAttachedFloor()->GetPlaceableGrids());
     			if(Count >= Placeables.Num())
     			{
     				LOG_ERR("Not enough placeable grids, %d", Count);
     				break;
     			}
     			if(Placeables[Count] == nullptr)
     			{
     				LOG_ERR("GameMode : Grid null %d", Count);
     				continue;
     			}
     			if(Element->CharacterSize == ECharacterSize::Large)
     			{
     				GridMan->SetLargeGridActive(true);
     			}

     			while(!GridMan->IsGridWalkable(Placeables[Count]->Index, true) && Count < Placeables.Num())
     			{
     				Count+=1;
     			}
     			if(!GridMan->IsGridWalkable(Placeables[Count]->Index, true))
     			{
     				LOG_ERR("GameMode : Grid null %d", Count);
     				continue;
     			}
     			FVector Location = GridMan->GetNavigationPoint(Placeables[Count]->Index);
     			GridMan->SetLargeGridActive(false);
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

     				Char->UpdateCurrentGridIndex();
     				Characters.Add(Char);
     			}
     		}
     		else
     		{
     			LOG_ERR("GameMode : Gridman or Gridfloor null");
     		}
     		Count+=1;
     	}

     	if(GridMan && GridMan->GetAttachedFloor())
     	{
     		GridMan->GetAttachedFloor()->ShowPlaceableGrids();
     	}
     }



	BattleTurnManager.SetCharactersList(Characters);
	BattleTurnManager.SetGameMode(this);
	//BattleTurnManager.Start();

	HudWidgetInstance = Cast<UBattleHudWidget>(WidgetManager::GetOrCreateWidgetInstance(EWidgetKeys::BattleHud, this));
    if(HudWidgetInstance)
    {
		BattleTurnManager.SetTurnWidget(HudWidgetInstance->GetTurnInfoWidget());
        HudWidgetInstance->AddToViewport();
    }
}

void AGuildGameGameModeBase::Start()
{
	ABattlePlayerController* PlayerController = Cast<ABattlePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if(PlayerController)
	{
		PlayerController->ChangeStateTo(EControllerStateIndex::Movement);
		BattleTurnManager.Start();
		PlayerController->SetSelectedCharacter(BattleTurnManager.GetCurrentCharacter());
	}
}

void AGuildGameGameModeBase::Next()
{
	ABattlePlayerController* PlayerController = Cast<ABattlePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if(PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Next()"));
		PlayerController->ChangeStateTo(EControllerStateIndex::Movement);
		BattleTurnManager.NextCharacter();
		PlayerController->SetSelectedCharacter(BattleTurnManager.GetCurrentCharacter());
	}
}

void AGuildGameGameModeBase::OnRoundEnds()
{
	for (int i = 0; i < Characters.Num(); ++i)
	{
		if(Characters[i])
		{
			Characters[i]->OnRoundEnds();
		}
	}

	if(HudWidgetInstance)
	{
		HudWidgetInstance->OnRoundEnds();
	}
}