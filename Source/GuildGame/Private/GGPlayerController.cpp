// Fill out your copyright notice in the Description page of Project Settings.


#include "GGPlayerController.h"

#include "AIController.h"
#include "GGCharacter.h"
#include "GGLogHelper.h"
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
	
	FVector WorldLocation;
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
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, "ok");
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT(" %d"), GridIndex));
	SelectedGridIndex = GridIndex;
	GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(GridIndex), true);
}

void AGGPlayerController::SetGridFloor(AGridFloor* Grid)
{
	GridFloor = Grid;
}

void AGGPlayerController::SelectCharAtMousePos()
{
	FHitResult TraceResult(ForceInit);
	this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceResult);
	if (TraceResult.GetActor() != nullptr)
	{	
		AGGCharacter* HitCharacter = Cast<AGGCharacter>(TraceResult.GetActor());			
		if(HitCharacter != nullptr)
		{
			SelectedCharacter = HitCharacter;
			CharacterManager::SetMovableGrids(SelectedCharacter);

			if(GridFloor)
			{
				GridFloor->UpdateGridMeshes(*(SelectedCharacter->GetMovableGrids()));
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "selected");
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "moving 1");
	GridManager* GridMan =  GridFloor->GetGridManager();
	if(GridMan == nullptr)
	{
		return;
	}
	//AController* aaaa = SelectedCharacter->GetController();
	AAIController* AIController = Cast<AAIController>(SelectedCharacter->GetController());
	if(AIController)
	{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "moving 2");
	LOG("x %d", GridMan->GetGridCenter(SelectedGridIndex).X);
	LOG("y %d", GridMan->GetGridCenter(SelectedGridIndex).Y);
	LOG("z %d", GridMan->GetGridCenter(SelectedGridIndex).Z);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT(" %d"), GridMan->GetGridCenter(SelectedGridIndex).X));
		AIController->MoveToLocation(GridMan->GetGridCenter(SelectedGridIndex),0,false,true,false,true,0,false);
	}
}

void AGGPlayerController::SetSelectedCharacter(AGGCharacter* NewCharacter)
{
	SelectedCharacter = NewCharacter;
}
