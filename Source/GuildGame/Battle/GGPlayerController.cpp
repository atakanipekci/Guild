// Fill out your copyright notice in the Description page of Project Settings.


#include "GGPlayerController.h"

#include "AIController.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GGControllerState.h"
#include "GuildGame/GridSystem/GridFloor.h"
#include "GGLogHelper.h"

AGGPlayerController::AGGPlayerController()
	:Super()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	PrimaryActorTick.bCanEverTick = true;
	States.Add(new ControllerStateDefault(this));
	States.Add(new ControllerStateBasicAttack(this));
	if(States.Num() != 0)
	{
		ActiveState = States[0];
	}
}

AGGPlayerController::~AGGPlayerController()
{
	for(int i = 0; i < States.Num(); i++)
	{
		delete States[i];
		States.RemoveAt(i);
	}
}


void AGGPlayerController::BeginPlay()
{

}

void AGGPlayerController::Tick(float DeltaTime)
{
	if(ActiveState)
	{
		ActiveState->Update();
	}
}

GGControllerState* AGGPlayerController::GetActiveState() const
{
	return ActiveState;
}

void AGGPlayerController::SetState(int StateIndex)
{
	if(StateIndex >= States.Num())
	{
		return;
	}

	ActiveState = States[StateIndex];
}

void AGGPlayerController::ChangeStateTo(int StateIndex)
{
	if(StateIndex >= States.Num())
	{
		return;
	}

	if(States[StateIndex])
	{
		if(!States[StateIndex]->CanChangeTo())
		{
			return;
		}
	}
	
	if(ActiveState)
	{
		ActiveState->ChangeFrom();
	}

	ActiveState = States[StateIndex];
	ActiveState->ChangeTo();
}

void AGGPlayerController::UpdateSelectedGrid(bool DrawPathTo)
{
	if(GridFloor == nullptr)
	{
		return;
	}
	
	GridManager* GridMan =  GridFloor->GetGridManager();
	if(GridMan == nullptr)
	{
		return;
	}
	
	FHitResult TraceResult(ForceInit);
	this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceResult);

	if(TraceResult.GetActor() == nullptr)
	{
		GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(0), false);
		return;
	}

	int GridIndex = GridMan->WorldToGrid(TraceResult.ImpactPoint);

	int MaxIndex = GridFloor->ColumnCount*GridFloor->RowCount -1;
	if(GridIndex < 0 || GridIndex > MaxIndex)
	{
		GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(0), false);
		return;
	}
	
	if(GridIndex!=SelectedGridIndex)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT(" %d"), GridIndex));
		SelectedGridIndex = GridIndex;
		GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(GridIndex), true);
		if(SelectedCharacter && SelectedCharacter->GetStatus() == ECharacterStatus::Idle && DrawPathTo)
		{
			int start = GridMan->WorldToGrid(SelectedCharacter->GetActorLocation());
			int end = SelectedGridIndex;
			GridFloor->DrawPath(start,end);
		}
	}
}

void AGGPlayerController::DrawPath(int StartIndex, int EndIndex) const
{
	if(StartIndex == EndIndex || GridFloor == nullptr)
	{
		return;
	}

	GridFloor->DrawPath(StartIndex,EndIndex);
}

void AGGPlayerController::SetGridFloor(AGridFloor* Grid)
{
	GridFloor = Grid;
}

AGridFloor* AGGPlayerController::GetGridFloor() const
{
	return GridFloor;
}

void AGGPlayerController::SelectCharAtMousePos()
{
	AGGCharacter* Char = GetCharacterFromMousePos();
	if(Char)
	{
		SetSelectedCharacter(Char);
	}
}

void AGGPlayerController::MoveSelectedChar()
{
	if(SelectedCharacter == nullptr || SelectedGridIndex < 0 || GridFloor == nullptr)
	{
		return;
	}
	GridManager* GridMan =  GridFloor->GetGridManager();
	if(GridMan == nullptr)
	{
		return;
	}

	float Dist = GridFloor->GetPathLength(GridMan->WorldToGrid(SelectedCharacter->GetActorLocation()), SelectedGridIndex);
	if(Dist <= 0 || Dist > SelectedCharacter->GetDefaultMovementRange())
	{
		return;
	}
	FVector Target = GridMan->GetGridCenter(SelectedGridIndex);
	Target.Z = SelectedCharacter->GetActorLocation().Z;
	SelectedCharacter->MoveTo(Target);
}

void AGGPlayerController::SetSelectedCharacter(AGGCharacter* NewCharacter)
{
	SelectedCharacter = NewCharacter;
	if(SelectedCharacter)
	{
		SelectedCharacter->SetSelected();
	}
}

AGGCharacter* AGGPlayerController::GetSelectedCharacter() const
{
	return SelectedCharacter;
}

AGGCharacter* AGGPlayerController::GetCharacterFromMousePos() const
{
	FHitResult TraceResult(ForceInit);
	this->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, false, TraceResult);
	if (TraceResult.GetActor() != nullptr)
	{	
		AGGCharacter* HitCharacter = Cast<AGGCharacter>(TraceResult.GetActor());			
		if(HitCharacter != nullptr)
		{
			return HitCharacter;
		}
		else
		{
			return nullptr;
		}
	}
	return nullptr;
}
