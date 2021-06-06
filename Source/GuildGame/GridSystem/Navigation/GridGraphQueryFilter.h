// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GuildGame/Managers/GridManager.h"

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

		static const FIntPoint kXOffset(1, 0);
        static const FIntPoint kYOffset(0, 1);

        if (NodeB.X < NodeA.X)
        {
            if (NodeB.Y < NodeA.Y)
            {
                if (!QueryGridManager->IsGridWalkable(NodeA - kXOffset) || !QueryGridManager->IsGridWalkable(NodeA - kYOffset))
                    return false;
            }
            else if (NodeB.Y > NodeA.Y)
            {
                if (!QueryGridManager->IsGridWalkable(NodeA - kXOffset) || !QueryGridManager->IsGridWalkable(NodeA + kYOffset))
                    return false;
            }
        }
        else if (NodeB.X > NodeA.X)
        {
            if (NodeB.Y < NodeA.Y)
            {
                if (!QueryGridManager->IsGridWalkable(NodeA + kXOffset) || !QueryGridManager->IsGridWalkable(NodeA - kYOffset))
                    return false;
            }
            else if (NodeB.Y > NodeA.Y)
            {
                if (!QueryGridManager->IsGridWalkable(NodeA + kXOffset) || !QueryGridManager->IsGridWalkable(NodeA + kYOffset))
                    return false;
            }
        }
    
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
