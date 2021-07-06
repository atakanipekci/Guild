// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleControllerState.h"
#include "BattlePlayerController.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/GridSystem/GridFloor.h"
#include "GGLogHelper.h"
#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Widgets/BattleHudWidget.h"
#include "GuildGame/GridSystem/Grid.h"
#include "GuildGame/Managers/CharacterManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"

ControllerStateDefault::ControllerStateDefault(ABattlePlayerController* Controller)
{
    PlayerController = Controller;
}

void ControllerStateDefault::Update()
{
	if(PlayerController == nullptr)
	{
		return;
	}

	PlayerController->UpdateSelectedGrid(true);

	AGGCharacter* SelectedChar = PlayerController->GetSelectedCharacter();
	if(SelectedChar && SelectedChar->GetStatus() == ECharacterStatus::Moving && PlayerController->GetGridFloor())
	{
		GridManager* GridMan = PlayerController->GetGridFloor()->GetGridManager();
		if(GridMan)
		{
			int NewGridIndex = GridMan->WorldToGrid(SelectedChar->GetActorLocation());

			
			if(LatestSelectedChar == SelectedChar && LatestGridIndex != NewGridIndex)
			{
				GridMan->ApplyGridStatusEffectsToCharacter(SelectedChar, NewGridIndex);
			}

			LatestGridIndex = NewGridIndex;
			LatestSelectedChar = SelectedChar;
		}
	}
	
}

void ControllerStateDefault::LeftClickHandler()
{
}

void ControllerStateDefault::LeftClickReleaseHandler()
{
    if (PlayerController == nullptr)
	{
		return;
	}

	//PlayerController->SelectCharAtMousePos();

	/*AGuildGameGameModeBase* GameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(PlayerController->GetWorld()));
	if(GameMode)
		GameMode->BattleTurnManager.Start();*/
}

void ControllerStateDefault::RightClickHandler()
{
}

void ControllerStateDefault::RightClickReleaseHandler()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->MoveSelectedChar();

	AGuildGameGameModeBase* BattleGameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(PlayerController));
	AGGCharacter* SelectedChar = PlayerController->GetSelectedCharacter();
	if(BattleGameMode && SelectedChar)
	{
		if(BattleGameMode->HudWidgetInstance)
		{
			BattleGameMode->HudWidgetInstance->RefreshApBar(SelectedChar->GetCurrentAP());
		}
	}

	/*AGuildGameGameModeBase* GameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(PlayerController->GetWorld()));
	if(GameMode)
		GameMode->BattleTurnManager.NextCharacter();*/
}

void ControllerStateDefault::ESCHandler()
{
}

void ControllerStateDefault::ChangeTo()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	AGGCharacter* SelecterChar = PlayerController->GetSelectedCharacter();
	if(SelecterChar)
	{
		SelecterChar->SetSelected();
	}
}

void ControllerStateDefault::ChangeFrom()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if(PlayerController->GetGridFloor())
	{
		PlayerController->GetGridFloor()->ClearGridMeshes();
		PlayerController->GetGridFloor()->ClearPath();
	}
}

bool ControllerStateDefault::CanChangeTo()
{
	return true;
}

ControllerStateCastingSkill::ControllerStateCastingSkill(ABattlePlayerController* Controller)
{
	PlayerController = Controller;
}

void ControllerStateCastingSkill::Update()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if(PlayerController->UpdateSelectedGrid(false))
	{
		if(PlayerController->GetSelectedCharacter() && PlayerController->GetSelectedCharacter()->GetStatus() == ECharacterStatus::Idle && PlayerController->GetGridFloor() && PlayerController->GetGridFloor()->GetGridManager())
		{
			GridManager* GridMan = PlayerController->GetGridFloor()->GetGridManager();
			AGGCharacter* Char = PlayerController->GetSelectedCharacter();
			TArray<Grid*>* Targetables = Char->GetTargetableGrids();

			if(GridMan->DoesInclude(Targetables, PlayerController->GetSelectedGridIndex()))
			{
				Char->ShowDamageableGrids(PlayerController->GetSelectedGridIndex(), false);
			}
			else
			{
				Char->ShowDamageableGrids(GridMan->GetClosestInArray(Targetables, PlayerController->GetSelectedGridIndex()), false);
			}

			UpdateHighlights();

			PlayerController->GetGridFloor()->DrawTrajectory(Char);

			BeginDamagePreview(Char);
		}
		else
		{
			StopDamagePreview();
			ResetHighlight();
		}
	}
}

void ControllerStateCastingSkill::LeftClickHandler()
{
}

void ControllerStateCastingSkill::LeftClickReleaseHandler()
{
	if (PlayerController != nullptr && PlayerController->GetSelectedCharacter() != nullptr)
	{
		AGGCharacter* SelectedCharacter = PlayerController->GetSelectedCharacter();
		if(SelectedCharacter && SelectedCharacter->GetStatus() != ECharacterStatus::Casting)
		{
			if(PlayerController->GetGridFloor())
			{
				GridManager* GridMan = PlayerController->GetGridFloor()->GetGridManager();
				if(GridMan)
				{
					FPredictProjectilePathResult Result;
					if(GridMan->CanAttackTargetGrid(SelectedCharacter, Result) && GridMan->DoesInclude(SelectedCharacter->GetDamageableGrids(), PlayerController->GetSelectedGridIndex()))
					{
						TArray<AGGCharacter*> Targets;
						GridMan->GetCharactersInArray(SelectedCharacter->GetDamageableGrids(), &Targets);

						TArray<AGGCharacter*> TargetsToEffect;
						GridMan->GetCharsInEffectSight(Targets, TargetsToEffect, SelectedCharacter, PlayerController->GetWorld());
						SelectedCharacter->CastSkill(TargetsToEffect);
						StopDamagePreview();
						ResetHighlight();

						AGuildGameGameModeBase* BattleGameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(PlayerController));
						if(BattleGameMode)
						{
							if(BattleGameMode->HudWidgetInstance)
							{
								BattleGameMode->HudWidgetInstance->RefreshApBar(SelectedCharacter->GetCurrentAP());
							}
						}
					}
				}
			}
		}	
	}

	//PlayerController->GetSelectedCharacter()
	
}

void ControllerStateCastingSkill::RightClickHandler()
{
}

void ControllerStateCastingSkill::RightClickReleaseHandler()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ChangeStateTo(EControllerStateIndex::Movement);

	if(PlayerController->GetSelectedCharacter())
	{
		PlayerController->GetSelectedCharacter()->ShowMovableGrids(true);
	}
	AGuildGameGameModeBase* BattleGameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(PlayerController));
	if(BattleGameMode)
	{
		if(BattleGameMode->HudWidgetInstance)
		{
			BattleGameMode->HudWidgetInstance->RefreshSkillButtonsState();
		}
	}
}

void ControllerStateCastingSkill::ESCHandler()
{
	RightClickReleaseHandler();
}

void ControllerStateCastingSkill::ChangeTo()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	
	AGGCharacter* SelectedCharacter = PlayerController->GetSelectedCharacter();
	if(SelectedCharacter)
	{
		SelectedCharacter->ShowTargetableGrids();
		TArray<Grid*>* Targetables = SelectedCharacter->GetTargetableGrids();
		if(PlayerController->GetGridFloor() && PlayerController->GetGridFloor()->GetGridManager())
		{
			GridManager* GridMan = PlayerController->GetGridFloor()->GetGridManager();
			SelectedCharacter->ShowDamageableGrids(GridMan->GetClosestInArray(Targetables, PlayerController->GetSelectedGridIndex()));

			PlayerController->GetGridFloor()->DrawTrajectory(SelectedCharacter);
		}

		if (SelectedCharacter->OnSkillChangeDelegate.IsBound() == false)
		{
			SelectedCharacter->OnSkillChangeDelegate.BindRaw(this, &ControllerStateCastingSkill::OnCurrentSkillChange);
		}

		AGuildGameGameModeBase* BattleGameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(PlayerController));
		if(BattleGameMode)
		{
			if(BattleGameMode->HudWidgetInstance)
			{
				BattleGameMode->HudWidgetInstance->PreviewSpending(SelectedCharacter->GetApCostOfCurrentSkill());
			}
		}
	}

	UpdateHighlights();
}

void ControllerStateCastingSkill::ChangeFrom()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if(PlayerController->GetGridFloor())
	{
		PlayerController->GetGridFloor()->ClearGridMeshes();
		PlayerController->GetGridFloor()->ClearTrajectory();
	}

	StopDamagePreview();
	ResetHighlight();
}

bool ControllerStateCastingSkill::CanChangeTo()
{
	if (PlayerController == nullptr)
	{
		return false;
	}

	if (PlayerController->GetSelectedCharacter() == nullptr || PlayerController->GetSelectedCharacter()->GetStatus() != ECharacterStatus::Idle)
	{
		return false;
	}

	return true;
}

void ControllerStateCastingSkill::BeginDamagePreview(AGGCharacter* SelectedChar)
{
	if(SelectedChar == nullptr || PlayerController == nullptr || PlayerController->GetGridFloor() == nullptr) return;

	StopDamagePreview();
	
	GridManager* GridMan = PlayerController->GetGridFloor()->GetGridManager();

	if(GridMan == nullptr) return;
	
	TArray<AGGCharacter*> Targets;
	GridMan->GetCharactersInArray(SelectedChar->GetDamageableGrids(), &Targets);

	TArray<AGGCharacter*> TargetsToEffect;
	GridMan->GetCharsInEffectSight(Targets, TargetsToEffect, SelectedChar, PlayerController->GetWorld());

	for (int i = 0; i < TargetsToEffect.Num(); ++i)
	{
		if(TargetsToEffect[i])
		{
			float HpDamage = 0;
			float ArmorDamage = 0;
			float MagicArmorDamage = 0;
			SelectedChar->GetCurrentSkillDamage(HpDamage, ArmorDamage, MagicArmorDamage);

			TargetsToEffect[i]->BeginDamagePreview(HpDamage, ArmorDamage, MagicArmorDamage);
			DamagePreviewedCharacters.Add(TargetsToEffect[i]);
		}
	}
}

void ControllerStateCastingSkill::StopDamagePreview()
{
	for (int i = 0; i < DamagePreviewedCharacters.Num(); ++i)
	{
		if(DamagePreviewedCharacters[i])
		{
			DamagePreviewedCharacters[i]->StopDamagePreview();
		}
	}
	DamagePreviewedCharacters.Empty();
}

void ControllerStateCastingSkill::UpdateHighlights()
{
	if(PlayerController == nullptr || PlayerController->GetGridFloor() == nullptr || PlayerController->GetGridFloor()->GetGridManager() == nullptr || PlayerController->GetSelectedCharacter() == nullptr) return;

	GridManager* GridMan = PlayerController->GetGridFloor()->GetGridManager();
	AGGCharacter* Char = PlayerController->GetSelectedCharacter();
	TArray<Grid*>* Damagables = Char->GetDamageableGrids();
	TArray<AGGCharacter*> Targets;
	TArray<AGGCharacter*> TargetsToEffect;

	ResetHighlight();

	GridMan->GetCharactersInArray(Damagables, &Targets);
	GridMan->GetCharsInEffectSight(Targets, TargetsToEffect, Char, PlayerController->GetWorld());

	for (int i = 0; i < TargetsToEffect.Num(); ++i)
	{
		if(TargetsToEffect[i])
		{
			TargetsToEffect[i]->SetCustomDepth(true,3);
			HighlightedCharacters.Add(TargetsToEffect[i]);
		}
	}
}

void ControllerStateCastingSkill::ResetHighlight()
{
	for (int i = HighlightedCharacters.Num() - 1; i >= 0; --i)
	{
		if(HighlightedCharacters[i])
		{
			HighlightedCharacters[i]->SetCustomDepth(false,1);
		}
		HighlightedCharacters.RemoveAt(i);
	}
}

void ControllerStateCastingSkill::OnCurrentSkillChange()
{
	if (PlayerController != nullptr)
	{
		AGGCharacter* SelectedCharacter = PlayerController->GetSelectedCharacter();
		if(SelectedCharacter)
		{
			BeginDamagePreview(SelectedCharacter);
		}
	}
}



ControllerStatePlacement::ControllerStatePlacement(ABattlePlayerController* Controller)
{
	PlayerController = Controller;
}

void ControllerStatePlacement::Update()
{
}

void ControllerStatePlacement::LeftClickHandler()
{
}

void ControllerStatePlacement::LeftClickReleaseHandler()
{
	if (PlayerController == nullptr || PlayerController->GetGridFloor() == nullptr)
	{
		return;
	}

	AGGCharacter* Old = SelectedCharacter;
	SelectedCharacter = PlayerController->GetCharacterFromMousePos();
	if(SelectedCharacter)
	{
		CharacterManager::SetCharacterGrids(SelectedCharacter, EGridState::Empty);
		SelectedCharacter->SetCustomDepth(true,2);
		if(PlayerController->GetGridFloor()->GetGridManager())
		{
			if(SelectedCharacter->GetSize() == ECharacterSize::Large)
			{
				PlayerController->GetGridFloor()->GetGridManager()->SetLargeGridActive(true);
			}
			else
			{
				PlayerController->GetGridFloor()->GetGridManager()->SetLargeGridActive(false);
			}
		}
	}
	if(Old)
	{
		Old->UpdateCurrentGridIndex();
		Old->SetCustomDepth(false,0);
	}
	
	PlayerController->GetGridFloor()->ShowPlaceableGrids();
}

void ControllerStatePlacement::RightClickHandler()
{
}

void ControllerStatePlacement::RightClickReleaseHandler()
{
	if(SelectedCharacter != nullptr && PlayerController != nullptr && SelectedCharacter != nullptr)
	{
		if(PlayerController->GetGridFloor() && PlayerController->GetGridFloor()->GetGridManager())
		{
			GridManager* GridMan = PlayerController->GetGridFloor()->GetGridManager();
			if(GridMan == nullptr)
			{
				return;
			}
		
			FHitResult TraceResult(ForceInit);
			PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceResult);
			if (TraceResult.IsValidBlockingHit())
			{
				FVector Location = TraceResult.Location;
				if(SelectedCharacter->GetSize() == ECharacterSize::Large)
				{
					Location = GridMan->GetClosestLineIntersection(Location);
				}
				int Index = GridMan->WorldToGrid(Location);
				if(GridMan->IsGridWalkable(Index))
				{
					if(GridMan->DoesInclude(&(PlayerController->GetGridFloor()->GetPlaceableGrids()), Index))
					{
						SelectedCharacter->TeleportTo(GridMan->GetNavigationPoint(Index), SelectedCharacter->GetActorRotation());
						//SelectedCharacter->UpdateCurrentGridIndex();
						PlayerController->GetGridFloor()->ShowPlaceableGrids();
					}
				}
			}
		}
	}
}

void ControllerStatePlacement::ESCHandler()
{
}

void ControllerStatePlacement::ChangeTo()
{
}

void ControllerStatePlacement::ChangeFrom()
{
	if(SelectedCharacter)
	{
		SelectedCharacter->SetCustomDepth(false, 1);
		SelectedCharacter->UpdateCurrentGridIndex();
	}
}

bool ControllerStatePlacement::CanChangeTo()
{
	return false;
}
