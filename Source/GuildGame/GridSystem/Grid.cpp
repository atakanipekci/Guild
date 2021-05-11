// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

Grid::Grid(int ArrayIndex, EGridState NewState)
{
    Index = ArrayIndex;
    GridState = NewState;
}

Grid::~Grid()
{
}
