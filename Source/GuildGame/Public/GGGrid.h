// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

enum class EGridState
{
	Empty = 0,
	Obstacle = 1,
	HalfObstacle = 2
};

enum class EGridEdge
{
	Empty = 0,
	Cover = 1,
	HalfCover = 2,
	Obstacle = 3,
	HalfObstacle = 4
	
};

class GUILDGAME_API GGGrid
{
public:
	GGGrid(int ArrayIndex, EGridState NewState);
	~GGGrid();
	int Index;
	EGridState GridState = EGridState::Empty;
	EGridEdge GridEdge[4];
};
