// Fill out your copyright notice in the Description page of Project Settings.


#include "GridNavigationData.h"
#include "AI/Navigation/NavigationTypes.h"
#include "EngineUtils.h"
#include "GridFloor.h"
#include "GridGraphQueryFilter.h"

AGridNavigationData::AGridNavigationData(const FObjectInitializer&)
{
    if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		FindPathImplementation = FindPath;
		FindHierarchicalPathImplementation = FindPath;

		Pathfinder = new FGraphAStar<GridGraph>(Graph);
	}
}

void AGridNavigationData::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AGridFloor> It(GetWorld(), AGridFloor::StaticClass()); It; ++It)
	{
		NavGridManager = It->GetGridManager();
		Graph.SetGridManager(NavGridManager);
		break;
	}
}

bool AGridNavigationData::ProjectPoint(const FVector& Point, FNavLocation& OutLocation, const FVector& Extent, FSharedConstNavQueryFilter Filter, const UObject* Querier) const
{
	if (NavGridManager->IsGridValid(NavGridManager->WorldToGrid(Point)))
	{
		OutLocation = FNavLocation(Point);
		return true;
	}

	return false;
}

FPathFindingResult AGridNavigationData::FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query)
{
	const ANavigationData* Self = Query.NavData.Get();
	const AGridNavigationData* AStar = (const AGridNavigationData*)Self;
	check(AStar != nullptr);

	if (AStar == nullptr)
		return ENavigationQueryResult::Error;

	FPathFindingResult Result(ENavigationQueryResult::Error);
	Result.Path = Query.PathInstanceToFill.IsValid() ? Query.PathInstanceToFill : Self->CreatePathInstance<FNavigationPath>(Query);

	FNavigationPath* NavPath = Result.Path.Get();

	if (NavPath != nullptr)
	{
		if ((Query.StartLocation - Query.EndLocation).IsNearlyZero())
		{
			Result.Path->GetPathPoints().Reset();
			Result.Path->GetPathPoints().Add(FNavPathPoint(Query.EndLocation));
			Result.Result = ENavigationQueryResult::Success;
		}
		else if (Query.QueryFilter.IsValid())
		{
			FIntPoint myGridPos, targetGridPos;

			myGridPos = AStar->NavGridManager->WorldToGrid_Point(Query.StartLocation);
			targetGridPos = AStar->NavGridManager->WorldToGrid_Point(Query.EndLocation);

			GridGraphQueryFilter queryFilter;
			queryFilter.SetGridManager(AStar->NavGridManager);
			queryFilter.SetAllowPartialPaths(Query.bAllowPartialPaths);

			TArray<FIntPoint> Path;
			EGraphAStarResult aStarResult = AStar->Pathfinder->FindPath(myGridPos, targetGridPos, queryFilter, Path);

			if (aStarResult == EGraphAStarResult::SearchFail || aStarResult == EGraphAStarResult::InfiniteLoop)
			{
				UE_LOG(LogTemp, Warning, TEXT("AWorldGridNavigationData A* fail: %d"), aStarResult);

				Result.Result = ENavigationQueryResult::Fail;
				return Result;
			}

			Path.Insert(myGridPos, 0);

			for (auto& point : Path)
			{
				NavPath->GetPathPoints().Add(FNavPathPoint(AStar->NavGridManager->GetGridCenter(AStar->NavGridManager->PointToIndex(point))));
			}

			UE_LOG(LogTemp, Warning, TEXT("WorldGridNav path (%d points):"), Path.Num());
			for (int i = 0; i < Path.Num(); i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s (%s)"), *Path[i].ToString(), *NavPath->GetPathPoints()[i].Location.ToString());
			}
			UE_LOG(LogTemp, Warning, TEXT("WorldGridNav path end"));

			NavPath->MarkReady();
			Result.Result = ENavigationQueryResult::Success;
		}
	}

	return Result;
}