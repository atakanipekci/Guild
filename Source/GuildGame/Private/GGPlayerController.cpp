// Fill out your copyright notice in the Description page of Project Settings.


#include "GGPlayerController.h"

#include "AIController.h"
#include "GGCharacter.h"
#include "GGLogHelper.h"

AGGPlayerController::AGGPlayerController()
	:Super()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	PrimaryActorTick.bCanEverTick = true;
}


void AGGPlayerController::BeginPlay()
{

}

void AGGPlayerController::Tick(float DeltaTime)
{
	UpdateSelectedGrid();	
}

void AGGPlayerController::UpdateSelectedGrid()
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
		if(SelectedCharacter && SelectedCharacter->GetStatus() == ECharacterStatus::Idle)
		{
			int start = GridMan->WorldToGrid(SelectedCharacter->GetActorLocation());
			int end = SelectedGridIndex;
			GridFloor->DrawPath(start,end);
		}
	}
}

void AGGPlayerController::SetGridFloor(AGridFloor* Grid)
{
	GridFloor = Grid;
}

void AGGPlayerController::SelectCharAtMousePos()
{
	FHitResult TraceResult(ForceInit);
	this->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, false, TraceResult);
	if (TraceResult.GetActor() != nullptr)
	{	
		AGGCharacter* HitCharacter = Cast<AGGCharacter>(TraceResult.GetActor());			
		if(HitCharacter != nullptr)
		{
			SetSelectedCharacter(HitCharacter);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "not char");
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TraceResult.GetActor()->GetName());
		}
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
