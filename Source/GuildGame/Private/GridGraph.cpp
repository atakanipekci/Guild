// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGraph.h"

void GridGraph::SetGridManager(GridManager* NewManager)
{
	GraphGridManager = NewManager;
}

int GridGraph::GetNeighbourCount(FNodeRef NodeRef) const
{
	return 4;
}

GridGraph::FNodeRef GridGraph::GetNeighbour(const FNodeRef NodeRef, const int Index) const
{
	return NodeRef + Neighbours[Index];
}

bool GridGraph::IsValidRef(FNodeRef NodeRef) const
{
	return GraphGridManager->IsGridValid(NodeRef);
}