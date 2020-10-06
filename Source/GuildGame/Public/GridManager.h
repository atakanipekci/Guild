// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GGGrid.h"

/**
 * 
 */
class GUILDGAME_API GridManager
{
public:
	GridManager(FVector2D StartPos,float GridSize,int ColumnCount, int RowCount);
	~GridManager();
	bool IsGridWalkable(int)const;
	bool IsGridWalkable(FIntPoint Point) const;
	bool IsGridValid(FIntPoint Point)const;
	bool IsGridValid(int Index)const;
	int WorldToGrid(FVector WorldPos)const;
	FIntPoint WorldToGrid_Point(FVector WorldPos)const;
	FIntPoint IndexToPoint(int Index)const;
	int PointToIndex(FIntPoint Point) const;
	FVector GetGridCenter(int Index)const;
	FVector GetGridTopMid(int Index)const;
	FVector GetGridLeftMid(int Index)const;
	FVector GetGridTopLeft(int Index)const;
	FVector GetGridBottomLeft(int Index)const;
	void SetGridState(int Index, EGridState NewState);
	void SetGridStates(TArray<GGGrid*> GridsToSet, EGridState NewState);
	TArray<GGGrid*> GetGridsFromCenter(int Index, int RowCount, int ColumnCount);
	TArray<GGGrid*> GetNeighbours(int Index, int RowCount, int ColumnCount);
	FVector GetPositionToPlace(int Index, int RowCount, int ColumnCount);
	bool IsPlaceable(TArray<GGGrid*> GridsToPlace, EGridState RequiredState)const;

	TArray<GGGrid> GGGrids;
private:
	FVector2D StartPos;
	int ColumnCount;
	int RowCount;
	int GridSize;
};
