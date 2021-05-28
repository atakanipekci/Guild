// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GuildGame/GridSystem/Grid.h"

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
	float GetDistBetween(int Index1, int Index2) const; 
	void SetGridState(int Index, EGridState NewState);
	bool IsPlaceable(TArray<Grid*>* GridsToPlace, EGridState RequiredState) const;
	bool GetGridsFromCenter(int Index, int ARowCount, int AColumnCount,  TArray<Grid*>* GridsResult);
	bool GetNeighbours(int Index, int ARowCount, int AColumnCount, TArray<Grid*>* GridsResult);
	bool GetGridsInRange(int CenterIndex, float Dist, TArray<Grid*>* GridsResult, bool UsePathfinding = true);
	int GetIntercept(TArray<Grid*>* Grids1, TArray<Grid*>* Grids2, TArray<Grid*>* GridsResult = nullptr);
	bool DoesInclude(TArray<Grid*>* Grids, int Index);
	int GetClosestInArray(TArray<Grid*>* Grids, int Index) const;
	bool GetCharactersInArray(TArray<Grid*>* Grids, TArray<class AGGCharacter*>* Characters) const;
	FVector GetPositionToPlace(int Index, int RowCount, int ColumnCount) const;
	bool SetGridStates(TArray<Grid*>* GridsToSet, EGridState NewState);
	void SetStartPos(FVector2D& NewPos)
	{
		StartPos = NewPos;	
	}

	TArray<Grid> GGGrids;
private:
	FVector2D StartPos;
	int ColumnCount;
	int RowCount;
	int GridSize;
	UPROPERTY()
	class AGridFloor* AttachedFloor = nullptr;
public:
	
	void SetAttachedFloor(AGridFloor* AAttachedFloor)
	{
		this->AttachedFloor = AAttachedFloor;
	}

	AGridFloor* GetAttachedFloor() const
	{
		return AttachedFloor;
	}

	int GetGridSize() const
	{
		return GridSize;
	}
};
