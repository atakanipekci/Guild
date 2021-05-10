// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphAStar.h"
#include "GridGraph.h"
#include "GuildGame/Managers/GridManager.h"
#include "NavigationData.h"
#include "GridNavigationData.generated.h"

/**
 * 
 */
UCLASS(config = Engine, defaultconfig, hidecategories = (Input, Rendering, Tags, "Utilities|Transformation", Actor, Layers, Replication), notplaceable)
class GUILDGAME_API AGridNavigationData : public ANavigationData
{
	GENERATED_BODY()
	AGridNavigationData(const FObjectInitializer&);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	static FPathFindingResult FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query);

	bool ProjectPoint(const FVector& Point, FNavLocation& OutLocation, const FVector& Extent, FSharedConstNavQueryFilter Filter = NULL, const UObject* Querier = NULL) const override;

private:

	GridManager* NavGridManager;
	GridGraph Graph;
	FGraphAStar<GridGraph>* Pathfinder;
	
};
