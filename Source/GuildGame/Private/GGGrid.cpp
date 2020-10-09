// Fill out your copyright notice in the Description page of Project Settings.


#include "GGGrid.h"

GGGrid::GGGrid(int ArrayIndex, EGridState NewState)
{
    Index = ArrayIndex;
    GridState = NewState;

    if(Index == 208)
    {
        GridState = EGridState::Obstacle;
    }
}

GGGrid::~GGGrid()
{
}
