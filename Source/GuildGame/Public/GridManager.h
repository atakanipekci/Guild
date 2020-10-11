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
	bool IsPlaceable(TArray<GGGrid*>* GridsToPlace, EGridState RequiredState) const;
	bool GetGridsFromCenter(int Index, int ARowCount, int AColumnCount,  TArray<GGGrid*>* GridsResult);
	bool GetNeighbours(int Index, int ARowCount, int AColumnCount, TArray<GGGrid*>* GridsResult);
	bool GetGridsInRange(int CenterIndex, float Dist, TArray<GGGrid*>* GridsResult);
	FVector GetPositionToPlace(int Index, int RowCount, int ColumnCount) const;
	bool SetGridStates(TArray<GGGrid*>* GridsToSet, EGridState NewState);
	void SetStartPos(FVector2D& NewPos)
	{
		StartPos = NewPos;	
	}

	TArray<GGGrid> GGGrids;
private:
	FVector2D StartPos;
	int ColumnCount;
	int RowCount;
	int GridSize;
	class AGridFloor* AttachedFloor = nullptr;
public:
	
	void SetAttachedFloor(AGridFloor* AAttachedFloor)
	{
		this->AttachedFloor = AAttachedFloor;
	}

	int GetGridSize() const
	{
		return GridSize;
	}
};
