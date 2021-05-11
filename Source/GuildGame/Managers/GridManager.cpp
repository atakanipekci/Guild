// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

#include "GGLogHelper.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GuildGame/GridSystem/GridFloor.h"

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
    if(Index < 0 || Index >= ColumnCount*RowCount || GGGrids[Index].GridState != EGridState::Empty)
    {
        return false;
    }
    
    return true;
}

bool GridManager::IsGridWalkable(FIntPoint Point) const
{
    if(IsGridValid(PointToIndex(Point)))
    {
        return true;
    }
    return false;
}

bool GridManager::IsGridValid(FIntPoint Point) const
{
    return IsGridValid(PointToIndex(Point));
}

bool GridManager::IsGridValid(int Index) const
{
    if(Index < 0 || Index >= ColumnCount*RowCount)
    {
        return false;
    }

    if(GGGrids[Index].GridState == EGridState::Obstacle)
    {
        return false;
    }
    return true;
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
    int row = 0;
    int col = 0;
    if(ColumnCount != 0)
    {
        row = Index/ColumnCount;
        col = Index%ColumnCount;
    }
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
    int row = 0;
    int col = 0;
    if(ColumnCount != 0)
    {
        row = Index/ColumnCount;
        col = Index%ColumnCount;
    }

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
    Grid* GridToSet = &GGGrids[Index];
    GridToSet->GridState = NewState;
}

bool GridManager::GetGridsFromCenter(int Index, int ARowCount, int AColumnCount,  TArray<Grid*>* GridsResult)
{
    //todo: add controls for the edges of the map
    if(Index < 0 || GridsResult == nullptr)
    {
        return false;  
    }
    GridsResult->Reserve(ARowCount*AColumnCount);

    int RowIndex = Index/this->ColumnCount;
    int ColIndex = Index%this->ColumnCount;
    int ColStart = ColIndex - AColumnCount/2;
    int RowStart = RowIndex - ARowCount/2;
    int StartIndex = RowStart*this->ColumnCount + ColStart;

    for(int i = 0; i < AColumnCount; i++)
    {
        for(int j = 0; j < ARowCount; j++)
        {
            int result = StartIndex + (j*this->ColumnCount) +  i;
            if(result >= 0 && result < ColumnCount*RowCount)
            {
                GridsResult->Add(&GGGrids[result]);
            }
        }
    }
    return true; 
}

bool GridManager::GetNeighbours(int Index, int ARowCount, int AColumnCount, TArray<Grid*>* GridsResult)
{
    if(Index < 0|| GridsResult == nullptr)
    {
        return false;  
    }
     GridsResult->Reserve((ARowCount+1)*2 + (AColumnCount+1)*2);

    int RowIndex = Index/this->ColumnCount;
    int ColIndex = Index%this->ColumnCount;
    int ColStart = ColIndex - (AColumnCount+1)/2;
    int RowStart = RowIndex - (ARowCount+1)/2;
    int StartIndex = RowStart*this->ColumnCount + ColStart;


    //Right
    for(int i = 0; i < ARowCount; i++)
    {
        int result = StartIndex + ColumnCount + AColumnCount + 1 + i*ColumnCount;
        if(result >= 0 && result < ColumnCount*RowCount)
        {
            GridsResult->Add(&GGGrids[StartIndex + ColumnCount + AColumnCount + 1 + i*ColumnCount]);
        }
    }
    //Bottom
    for(int i = 0; i < AColumnCount+2; i++)
    {
        int result = StartIndex+(ColumnCount*(ARowCount+1)) + i;
        if(result >= 0 && result < ColumnCount*RowCount)
        {
            GridsResult->Add(&GGGrids[result]);
        }
    }
    //Left
    for(int i = 0; i < ARowCount; i++)
    {
        int result = StartIndex + ColumnCount + i*ColumnCount;
        if(result >= 0 && result < ColumnCount*RowCount)
        {
            GridsResult->Add(&GGGrids[result]);
        }
     }
    //Top
    for(int i = 0; i < AColumnCount+2; i++)
    {
        int result = StartIndex + i;

        if(result >= 0 && result < ColumnCount*RowCount)
        {
            GridsResult->Add(&GGGrids[result]);
        }
    }
    return true;   
}

bool GridManager::GetGridsInRange(int CenterIndex, float Dist, TArray<Grid*>* GridsResult, bool UsePathfinding)
{
    int IndexDist = Dist/GridSize;
    int TopLeft = CenterIndex - IndexDist - IndexDist*ColumnCount;

    FVector start = GetGridCenter(CenterIndex);
    FVector end;
    int result = 0;
    for(int i = 0; i <= IndexDist*2; i++)
    {
        for(int j = 0; j<= IndexDist*2; j++)
        {
            result = TopLeft + i + j*ColumnCount;

            
            if(result >= 0 && result < RowCount*ColumnCount && AttachedFloor)
            {
                end = GetGridCenter(result);
                if(IsGridValid(result))
                {
                    if(UsePathfinding)
                    {
                        UNavigationPath* path =  UNavigationSystemV1::FindPathToLocationSynchronously(AttachedFloor->GetWorld(), start, end, nullptr,nullptr);
                        if(path)
                        {
                            float PathDist = path->GetPathLength();
                            if(PathDist<Dist && path->IsValid())
                            {
                                GridsResult->Add(&GGGrids[result]);
                            }
                        }
                    }
                    else
                    {
                        float EuclidDist = FVector::Dist(start,end);
                        if(EuclidDist < Dist)
                        {
                            GridsResult->Add(&GGGrids[result]);
                        }
                    }
                }
            }
        }
    }
    return true;
}

FVector GridManager::GetPositionToPlace(int Index, int ARowCount, int AColumnCount) const
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

bool GridManager::IsPlaceable(TArray<Grid*>* GridsToPlace, EGridState RequiredState) const
{
    if(GridsToPlace == nullptr)
    {
        return false;  
    }
    
    for(int i = 0; i < GridsToPlace->Num(); i++)
    {
        if((*GridsToPlace)[i] == nullptr || (*GridsToPlace)[i]->GridState != RequiredState)
        {
            return false;
        }
    }

    return true;
}

bool GridManager::SetGridStates(TArray<Grid*>* GridsToSet, EGridState NewState)
{

    if(GridsToSet == nullptr)
    {
        return false;  
    }
    
    for(int i = 0; i < GridsToSet->Num(); i++)
    {
        if((*GridsToSet)[i] == nullptr)
        {
            continue;
        }
        (*GridsToSet)[i]->GridState = NewState;
    }

    return true;

}
