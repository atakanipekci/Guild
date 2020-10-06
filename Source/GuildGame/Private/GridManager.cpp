// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "GGLogHelper.h"

GridManager::GridManager(FVector2D StartPos, float GridSize, int ColumnCount, int RowCount)
{
    GGGrids.Reserve(ColumnCount*RowCount);
    for(int i = 0; i < ColumnCount*RowCount; i++)
    {
        GGGrids.Emplace(i,EGridState::Empty);
    }
    this->StartPos = StartPos;
    this->GridSize = GridSize;
    this->ColumnCount = ColumnCount;
    this->RowCount = RowCount;
}

GridManager::~GridManager()
{
}

bool GridManager::IsGridWalkable(int Index) const
{
    int row = Index/ColumnCount;
    int col = Index%ColumnCount;

    if(Index < 0 || Index > ColumnCount*RowCount || GGGrids[Index].GridState != EGridState::Empty)
    {
        return false;
    }
    
    return true;
}

bool GridManager::IsGridWalkable(FIntPoint Point) const
{
    return IsGridValid(PointToIndex(Point));
}

bool GridManager::IsGridValid(FIntPoint Point) const
{
    return IsGridValid(PointToIndex(Point));
}

bool GridManager::IsGridValid(int Index) const
{
    return !(Index < 0 || Index > ColumnCount*RowCount);
}

int GridManager::WorldToGrid(FVector WorldPos) const
{
    int row = (WorldPos.X - StartPos.X)/GridSize;
    int col = (WorldPos.Y - StartPos.Y)/GridSize;
    if(row >= RowCount || col >= ColumnCount || row < 0 || col < 0 || WorldPos.Y < StartPos.Y || WorldPos.X < StartPos.X)
    {
        return -1;
    }
    
    return row*ColumnCount + col;
}

FIntPoint GridManager::WorldToGrid_Point(FVector WorldPos) const
{
    int row = (WorldPos.X - StartPos.X)/GridSize;
    int col = (WorldPos.Y - StartPos.Y)/GridSize;
    if(row >= RowCount || col >= ColumnCount || row < 0 || col < 0 || WorldPos.Y < StartPos.Y || WorldPos.X < StartPos.X)
    {
        return -1;
    }

    return FIntPoint(col,row);
}

FIntPoint GridManager::IndexToPoint(int Index) const
{
    int row = Index/ColumnCount;
    int col = Index%ColumnCount;
    return FIntPoint(row,col);
}

int GridManager::PointToIndex(FIntPoint Point) const
{
    int row = Point.Y;
    int col = Point.X;

    return row*ColumnCount + col;
}

FVector GridManager::GetGridCenter(int Index) const
{
    int row = Index/ColumnCount;
    int col = Index%ColumnCount;

    //GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("  row : %d"), row));

    return FVector(StartPos.X + row*GridSize + GridSize/2, StartPos.Y + col*GridSize + GridSize/2,0);
}

FVector GridManager::GetGridTopMid(int Index) const
{
    return  GetGridCenter(Index) + FVector(GridSize/2,0,0);
}

FVector GridManager::GetGridLeftMid(int Index) const
{
    return  GetGridCenter(Index) + FVector(0,-GridSize/2,0);
}

FVector GridManager::GetGridTopLeft(int Index) const
{
    return GetGridCenter(Index) + FVector(GridSize/2,-GridSize/2,0);
}

FVector GridManager::GetGridBottomLeft(int Index) const
{

    //int row = Index/ColumnCount;
    //int col = Index%ColumnCount;

    //return FVector(StartPos.X - row*GridSize - GridSize, StartPos.Y + col*GridSize,0);
    return GetGridCenter(Index) + FVector(-GridSize/2,-GridSize/2,0);
}

void GridManager::SetGridState(int Index, EGridState NewState)
{
    if(GGGrids.Num() <= 0)
    {
        return;
    }
    GGGrid* GridToSet = &GGGrids[Index];
    GridToSet->GridState = NewState;
}

TArray<GGGrid*> GridManager::GetGridsFromCenter(int Index, int ARowCount, int AColumnCount)
{
    //todo: add controls for the edges of the map

    
    TArray<GGGrid*> GridsResult;
    if(Index < 0)
    {
        return GridsResult;  
    }
    GridsResult.Reserve(ARowCount*AColumnCount);

    int RowIndex = Index/this->ColumnCount;
    int ColIndex = Index%this->ColumnCount;
    int ColStart = ColIndex - AColumnCount/2;
    int RowStart = RowIndex - ARowCount/2;
    int StartIndex = RowStart*this->ColumnCount + ColStart;

    for(int i = 0; i < AColumnCount; i++)
    {
        for(int j = 0; j < ARowCount; j++)
        {
            GridsResult.Add(&GGGrids[StartIndex + (j*this->ColumnCount) +  i]);                
        }
    }
    return GridsResult; 
}

TArray<GGGrid*> GridManager::GetNeighbours(int Index, int ARowCount, int AColumnCount)
{
    TArray<GGGrid*> GridsResult;
    if(Index < 0)
    {
        return GridsResult;  
    }
     GridsResult.Reserve((ARowCount+1)*2 + (AColumnCount+1)*2);

    int RowIndex = Index/this->ColumnCount;
    int ColIndex = Index%this->ColumnCount;
    int ColStart = ColIndex - (AColumnCount+1)/2;
    int RowStart = RowIndex - (ARowCount+1)/2;
    int StartIndex = RowStart*this->ColumnCount + ColStart;


    //Right
    for(int i = 0; i < ARowCount; i++)
    {
        GridsResult.Add(&GGGrids[StartIndex + ColumnCount + AColumnCount + 1 + i*ColumnCount]);
    }
    //Bottom
    for(int i = 0; i < AColumnCount+2; i++)
    {
        GridsResult.Add(&GGGrids[StartIndex+(ColumnCount*(ARowCount+1)) + i]);
    }
    //Left
    for(int i = 0; i < ARowCount; i++)
    {
        GridsResult.Add(&GGGrids[StartIndex + ColumnCount + i*ColumnCount]);
    }
    //Top
    for(int i = 0; i < AColumnCount+2; i++)
    {
        GridsResult.Add(&GGGrids[StartIndex + i]);
    }
    return GridsResult;   
}

FVector GridManager::GetPositionToPlace(int Index, int ARowCount, int AColumnCount)
{
    if(ARowCount%2 == 0 && AColumnCount %2 == 0)
    {
        return GetGridTopLeft(Index);
    }
    else if(ARowCount%2 == 0)
    {
        return GetGridTopMid(Index);
    }
    else if(AColumnCount %2 == 0)
    {
        return GetGridLeftMid(Index);
    }
    else
    {
        return GetGridCenter(Index);
    }
}

bool GridManager::IsPlaceable(TArray<GGGrid*> GridsToPlace, EGridState RequiredState) const
{
    for(int i = 0; i < GridsToPlace.Num(); i++)
    {
        if(GridsToPlace[i] == nullptr || GridsToPlace[i]->GridState != RequiredState)
        {
            return false;
        }
    }

    return true;
}

void GridManager::SetGridStates(TArray<GGGrid*> GridsToSet, EGridState NewState)
{
    for(int i = 0; i < GridsToSet.Num(); i++)
    {
        if(GridsToSet[i] == nullptr)
        {
            continue;
        }
        GridsToSet[i]->GridState = NewState;
    }
}
