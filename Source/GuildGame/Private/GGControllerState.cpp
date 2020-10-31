// Fill out your copyright notice in the Description page of Project Settings.


#include "GGControllerState.h"
#include "GGPlayerController.h"
#include "GGCharacter.h"
#include "GGLogHelper.h"
#include "GridFloor.h"

ControllerStateDefault::ControllerStateDefault(AGGPlayerController* Controller)
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

ControllerStateBasicAttack::ControllerStateBasicAttack(AGGPlayerController* Controller)
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
		SelectedCharacter->ShowDamageableGrids();
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
