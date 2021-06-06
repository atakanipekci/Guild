// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GuildGame/Managers/GridManager.h"

/**
 * 
 */
class GUILDGAME_API GridGraph
{
public:

	void SetGridManager(GridManager* NewManager);

	typedef FIntPoint FNodeRef;
	int GetNeighbourCount(FNodeRef NodeRef) const;

	FNodeRef GetNeighbour(const FNodeRef NodeRef, const int Index) const;

	bool IsValidRef(FNodeRef NodeRef) const;
	
private:
	const FNodeRef Neighbours[8] =
	{
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 1, -1 },
		{ 0, -1 },
		{ -1, -1 },
		{ -1, 0 },
		{ -1, 1 },
	};
	GridManager* GraphGridManager = nullptr;
};
