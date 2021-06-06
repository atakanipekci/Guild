// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleControllerState.h"
#include "BattlePlayerController.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/GridSystem/GridFloor.h"
#include "GGLogHelper.h"
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

	PlayerController->SelectCharAtMousePos();
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
	
	if(PlayerController->GetSelectedCharacter())
	{
		PlayerController->GetSelectedCharacter()->SetSelected();
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

ControllerStateBasicAttack::ControllerStateBasicAttack(ABattlePlayerController* Controller)
{
	 PlayerController = Controller;
}

void ControllerStateBasicAttack::Update()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->UpdateSelectedGrid(false);
}

void ControllerStateBasicAttack::LeftClickHandler()
{
}

void ControllerStateBasicAttack::LeftClickReleaseHandler()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	AGGCharacter* Char = PlayerController->GetCharacterFromMousePos();
	if(Char != nullptr && Char != PlayerController->GetSelectedCharacter())
	{
		if(PlayerController->GetSelectedCharacter())
		{
			PlayerController->GetSelectedCharacter()->AttackTo(Char);
		}
	}
}

void ControllerStateBasicAttack::RightClickHandler()
{
	
}

void ControllerStateBasicAttack::RightClickReleaseHandler()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ChangeStateTo(0);
}

void ControllerStateBasicAttack::ESCHandler()
{
	RightClickReleaseHandler();
}

void ControllerStateBasicAttack::ChangeTo()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	AGGCharacter* SelectedCharacter = PlayerController->GetSelectedCharacter();
	if(SelectedCharacter)
	{
		SelectedCharacter->ShowTargetableGrids();
	}
}

void ControllerStateBasicAttack::ChangeFrom()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if(PlayerController->GetGridFloor())
	{
		PlayerController->GetGridFloor()->ClearGridMeshes();
	}
}

bool ControllerStateBasicAttack::CanChangeTo()
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
		if(PlayerController->GetSelectedCharacter() && PlayerController->GetSelectedCharacter()->GetStatus() == ECharacterStatus::Idle)
		{
			if(PlayerController->GetGridFloor() && PlayerController->GetGridFloor()->GetGridManager())
			{
				GridManager* GridMan = PlayerController->GetGridFloor()->GetGridManager();
				TArray<Grid*>* Targetables = PlayerController->GetSelectedCharacter()->GetTargetableGrids();
				AGGCharacter* Char = PlayerController->GetSelectedCharacter();

				if(Char)
				{
					if(GridMan->DoesInclude(Targetables, PlayerController->GetSelectedGridIndex()))
					{
						Char->ShowDamageableGrids(PlayerController->GetSelectedGridIndex(), false);
					}
					else
					{
						Char->ShowDamageableGrids(GridMan->GetClosestInArray(Targetables, PlayerController->GetSelectedGridIndex()), false);
					}

					PlayerController->GetGridFloor()->DrawTrajectory(Char);
				}
				
			}
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
					if(GridMan->CanAttackTargetGrid(SelectedCharacter, Result))
					{
						TArray<AGGCharacter*> Targets;
						GridMan->GetCharactersInArray(SelectedCharacter->GetDamageableGrids(), &Targets);

						TArray<AGGCharacter*> TargetsToEffect;
						GridMan->GetCharsInEffectSight(Targets, TargetsToEffect, SelectedCharacter, PlayerController->GetWorld());
						SelectedCharacter->CastSkill(TargetsToEffect);
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

	PlayerController->ChangeStateTo(0);
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
	}
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