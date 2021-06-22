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
	bool IsGridWalkable(int, bool CheckLargeGrid = true)const;
	bool IsGridWalkable(FIntPoint Point, bool CheckLargeGrid = true) const;
	bool IsGridValid(FIntPoint Point)const;
	bool IsGridValid(int Index)const;
	int WorldToGrid(FVector WorldPos)const;
	FIntPoint WorldToGrid_Point(FVector WorldPos)const;
	FIntPoint IndexToPoint(int Index)const;
	int PointToIndex(FIntPoint Point) const;
	FVector GetNavigationPoint(int Index) const;
	FVector GetGridCenter(int Index)const;
	FVector GetGridTopMid(int Index)const;
	FVector GetGridLeftMid(int Index)const;
	FVector GetGridRightMid(int Index) const;
	FVector GetGridTopLeft(int Index)const;
	FVector GetGridTopRight(int Index)const;
	FVector GetGridBottomMid(int Index) const;
	FVector GetGridBottomLeft(int Index)const;
	FVector GetGridBottomRight(int Index)const;
	FVector GetClosestLineIntersection(FVector& Pos) const;
	float GetDistBetween(int Index1, int Index2) const; 
	void SetGridState(int Index, EGridState NewState);
	bool IsPlaceable(TArray<Grid*>* GridsToPlace, EGridState RequiredState) const;
	bool GetGridsFromCenter(int Index, int ARowCount, int AColumnCount,  TArray<Grid*>* GridsResult, bool LargeGrid = false);
	bool GetGridsFromIntersection(int Index, int ARowCount, int AColumnCount,  TArray<Grid*>* GridsResult);
	bool GetNeighbours(int Index, int ARowCount, int AColumnCount, TArray<Grid*>* GridsResult);
	bool GetGridsInRange(int CenterIndex, float Dist, TArray<Grid*>* GridsResult, bool UsePathfinding = true);
	bool GetLargeCharacterGrids(int Index, TArray<Grid*>& Out) const;
	int GetIntercept(TArray<Grid*>* Grids1, TArray<Grid*>* Grids2, TArray<Grid*>* GridsResult = nullptr);
	bool DoesInclude(TArray<Grid*>* Grids, int Index);
	int GetClosestInArray(TArray<Grid*>* Grids, int Index) const;
	bool GetCharactersInArray(TArray<Grid*>* Grids, TArray<class AGGCharacter*>* Characters) const;
	AGGCharacter* GetCharacterByGridIndex(int GridIndex) const;
	
	bool CanAttackTargetGrid(AGGCharacter* Character, struct FPredictProjectilePathResult& ProjectileResult);
	bool CanAttackTargetGrid(AGGCharacter* Character, struct FPredictProjectilePathResult& ProjectileResult, FVector StartLocation);
	bool PredictAttackOnTargetGrid(AGGCharacter* Character, struct FPredictProjectilePathResult& ProjectileResult, FVector StartLocation);
	void GetCharsInEffectSight(TArray<AGGCharacter*> CharsInArea, TArray<AGGCharacter*>& OutCharsInSight, AGGCharacter* SelectedCharacter, UWorld* World);
	FVector GetPositionToPlace(int Index, int RowCount, int ColumnCount) const;
	bool SetGridStates(TArray<Grid*>* GridsToSet, EGridState NewState);
	void SetStartPos(FVector2D& NewPos)
	{
		StartPos = NewPos;	
	}

	void SetLargeGridActive(bool Active)
	{
		LargeGridActive = Active;
	}

	TArray<Grid> GGGrids;
private:
	FVector2D StartPos;
	int ColumnCount;
	int RowCount;
	int GridSize;

	bool LargeGridActive = false;
	
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
