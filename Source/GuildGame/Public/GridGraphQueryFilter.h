// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridManager.h"

/**
 * 
 */
class GUILDGAME_API GridGraphQueryFilter
{
public:
	GridGraphQueryFilter() : QueryGridManager(nullptr), AllowPartialPaths(false)
	{
		
	}

	void SetGridManager(GridManager* NewGridManager)
	{
		QueryGridManager = NewGridManager;
	}

	float GetHeuristicScale() const
	{
		return 1;
	}

	float GetHeuristicCost(const FIntPoint StartNodeRef, const FIntPoint EndNodeRef) const
    {
        return FMath::Abs(StartNodeRef.X - EndNodeRef.X) + FMath::Abs(StartNodeRef.Y - EndNodeRef.Y);
    }

    float GetTraversalCost(const FIntPoint StartNodeRef, const FIntPoint EndNodeRef) const
    {
        return 1;
    }

	bool IsTraversalAllowed(const FIntPoint NodeA, const FIntPoint NodeB) const
	{
		if (!QueryGridManager->IsGridWalkable(NodeA) || !QueryGridManager->IsGridWalkable(NodeB))
			return false;

		return true;
	}

	bool WantsPartialSolution() const
	{
		return AllowPartialPaths;
	}

	void SetAllowPartialPaths(bool Allow)
	{
		AllowPartialPaths = Allow;
	}

private:
	GridManager* QueryGridManager;
	bool AllowPartialPaths;
};
