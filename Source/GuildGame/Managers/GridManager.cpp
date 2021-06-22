// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

#include "DrawDebugHelpers.h"
#include "GGLogHelper.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/GridSystem/GridFloor.h"
#include "GuildGame/GridSystem/GridObstacle.h"
#include "GuildGame/Skills/CharacterSkill.h"
#include "GuildGame/VFX/Projectiles/Projectile.h"
#include "Kismet/GameplayStatics.h"

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

bool GridManager::IsGridWalkable(int Index,  bool CheckLargeGrid) const
{
    if(Index < 0 || Index >= ColumnCount*RowCount || GGGrids[Index].GridState != EGridState::Empty)
    {
        return false;
    }

    if(LargeGridActive && CheckLargeGrid)
    {
        TArray<Grid*> Out;
        if(GetLargeCharacterGrids(Index, Out))
        {
            for (auto Element : Out)
            {
                if(Element == nullptr)
                {
                    return false;
                }

                if(Element->GridState != EGridState::Empty)
                {
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
    }
    
    return true;
}

bool GridManager::IsGridWalkable(FIntPoint Point,  bool CheckLargeGrid) const
{
    int Index = PointToIndex(Point);
    if(Index < 0 || Index >= ColumnCount*RowCount || GGGrids[Index].GridState != EGridState::Empty)
    {
        return false;
    }

    if(LargeGridActive && CheckLargeGrid)
    {
        TArray<Grid*> Out;
        if(GetLargeCharacterGrids(Index, Out))
        {
            for (auto Element : Out)
            {
                if(Element == nullptr)
                {
                    return false;
                }

                if(Element->GridState != EGridState::Empty)
                {
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
    }
    
    return true;
    
    /*if(IsGridValid(PointToIndex(Point)))
    {
        return true;
    }
    return false;*/
}

bool GridManager::IsGridValid(FIntPoint Point) const
{
    return (Point.X >= 0 && Point.Y >= 0 && Point.X < ColumnCount && Point.Y < RowCount);
    //return IsGridValid(PointToIndex(Point));
}

bool GridManager::IsGridValid(int Index) const
{
    if(Index < 0 || Index >= ColumnCount*RowCount)
    {
        return false;
    }

    /*if(GGGrids[Index].GridState == EGridState::Obstacle)
    {
        return false;
    }*/
    return true;
}

int GridManager::WorldToGrid(FVector WorldPos) const
{
    int row = (WorldPos.X - StartPos.X)/GridSize;
    int col = (WorldPos.Y - StartPos.Y)/GridSize;
    /*if(row >= RowCount || col >= ColumnCount || row < 0 || col < 0 || WorldPos.Y < StartPos.Y || WorldPos.X < StartPos.X)
    {
        return -1;
    }*/
    
    return row*ColumnCount + col;
}

FIntPoint GridManager::WorldToGrid_Point(FVector WorldPos) const
{
    int row = (WorldPos.X - StartPos.X)/GridSize;
    int col = (WorldPos.Y - StartPos.Y)/GridSize;
    /*if(row >= RowCount || col >= ColumnCount || row < 0 || col < 0 || WorldPos.Y < StartPos.Y || WorldPos.X < StartPos.X)
    {
        return -1;
    }*/

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
    return FIntPoint(col,row);
}

int GridManager::PointToIndex(FIntPoint Point) const
{
    int row = Point.Y;
    int col = Point.X;

    return row*ColumnCount + col;
}

FVector GridManager::GetNavigationPoint(int Index) const
{
    if(LargeGridActive)
    {
        return GetGridBottomLeft(Index);
    }
    else
    {
        return GetGridCenter(Index);
    }
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

FVector GridManager::GetGridRightMid(int Index) const
{
    return  GetGridCenter(Index) + FVector(0,+GridSize/2,0);
}

FVector GridManager::GetGridTopLeft(int Index) const
{
    return GetGridCenter(Index) + FVector(GridSize/2,-GridSize/2,0);
}

FVector GridManager::GetGridTopRight(int Index) const
{
    return GetGridCenter(Index) + FVector(GridSize/2,+GridSize/2,0);
}

FVector GridManager::GetGridBottomMid(int Index) const
{
    return  GetGridCenter(Index) + FVector(-GridSize/2,0,0);
}

FVector GridManager::GetGridBottomLeft(int Index) const
{

    //int row = Index/ColumnCount;
    //int col = Index%ColumnCount;

    //return FVector(StartPos.X - row*GridSize - GridSize, StartPos.Y + col*GridSize,0);
    return GetGridCenter(Index) + FVector(-GridSize/2,-GridSize/2,0);
}

FVector GridManager::GetGridBottomRight(int Index) const
{
    return GetGridCenter(Index) + FVector(-GridSize/2,+GridSize/2,0);
}

FVector GridManager::GetClosestLineIntersection(FVector& Pos) const
{
    float TopLeftDist = 0;
    float TopRightDist = 0;
    float BotLeftDist = 0;
    float BotRightDist = 0;

    int Index = WorldToGrid(Pos);
    
    TopLeftDist = FVector::Dist(Pos,GetGridTopLeft(Index));
    TopRightDist = FVector::Dist(Pos,GetGridTopRight(Index));
    BotLeftDist = FVector::Dist(Pos,GetGridBottomLeft(Index));

    //closer to right
    if(TopLeftDist > TopRightDist)
    {
        //closer to top
        if(BotLeftDist > TopLeftDist)
        {
            return GetGridTopRight(Index);
        }
        else//closer to bottom
        {
            return GetGridBottomRight(Index);
        }
    }
    else//closer to left
    {
        //closer to top
        if(BotLeftDist > TopLeftDist)
        {
            return GetGridTopLeft(Index);
        }
        else//closer to bottom
        {
            return GetGridBottomLeft(Index);
        }
    }
}

float GridManager::GetDistBetween(int Index1, int Index2) const
{
    return FVector::Dist(GetGridCenter(Index1), GetGridCenter(Index2));
}

void GridManager::SetGridState(int Index, EGridState NewState)
{
    if(GGGrids.Num() <= 0 || Index >= GGGrids.Num())
    {
        return;
    }
    Grid* GridToSet = &GGGrids[Index];
    GridToSet->GridState = NewState;
}

bool GridManager::GetGridsFromCenter(int Index, int ARowCount, int AColumnCount,  TArray<Grid*>* GridsResult, bool LargeGrid)
{
    //todo: add controls for the edges of the map
    if(Index < 0 || GridsResult == nullptr)
    {
        return false;  
    }

    int RowIndex = Index/this->ColumnCount;
    int ColIndex = Index%this->ColumnCount;
    int ColStart = ColIndex - AColumnCount/2;
    if(ColStart<0)
    {
        AColumnCount += ColStart; 
        ColStart = 0;
    }
    int RowStart = RowIndex - ARowCount/2;
    if(LargeGrid)
    {
        //RowStart -= 1;
        AColumnCount -= 1;
        ARowCount -= 1;
    }
    int StartIndex = RowStart*this->ColumnCount + ColStart;

    GridsResult->Reserve(ARowCount*AColumnCount);
    
    for(int i = 0; i < AColumnCount; i++)
    {
        if(ColStart + i >= ColumnCount)
        {
            break;
        }
        
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

bool GridManager::GetGridsFromIntersection(int Index, int ARowCount, int AColumnCount, TArray<Grid*>* GridsResult)
{
    if(Index < 0 || GridsResult == nullptr)
    {
        return false;  
    }

    FVector CenterPos = GetGridBottomLeft(Index);
    

    

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

    FVector start = GetNavigationPoint(CenterIndex);
    FVector end;
    int result = 0;
    for(int i = 0; i <= IndexDist*2; i++)
    {
        for(int j = 0; j<= IndexDist*2; j++)
        {
            result = TopLeft + i + j*ColumnCount;

            
            if(result >= 0 && result < RowCount*ColumnCount && AttachedFloor)
            {
                end = GetNavigationPoint(result);
                TArray<Grid*> CharGrids;
                if(LargeGridActive)
                {
                    if(!GetLargeCharacterGrids(CenterIndex,CharGrids))
                    {
                        //continue;
                    }
                    if(DoesInclude(&CharGrids, result))
                    {
                        //continue;
                    }
                }
                if(IsGridWalkable(result, false))
                {
                    if(UsePathfinding)
                    {
                        UNavigationPath* path =  UNavigationSystemV1::FindPathToLocationSynchronously(AttachedFloor->GetWorld(), start, end, nullptr,nullptr);
                        if(path)
                        {
                            float PathDist = path->GetPathLength();
                            if(PathDist<=Dist && path->IsValid())
                            {
                                int Num = path->PathPoints.Num();
                                if(Num > 1 && FVector::Dist(path->PathPoints[Num-1], GetNavigationPoint(result)) >= GridSize)
                                {
                                    continue;
                                }
                                else
                                {
                                    if(LargeGridActive == false)
                                    {
                                        GridsResult->Add(&GGGrids[result]);
                                    }
                                    else if(LargeGridActive)
                                    {
                                        TArray<Grid*> Neighbours;
                                        GetLargeCharacterGrids(result, Neighbours);
                                        for (auto Element : Neighbours)
                                        {
                                            if(Element == nullptr)
                                            {
                                                continue;
                                            }
                                            if(!DoesInclude(GridsResult, Element->Index) && !DoesInclude(&CharGrids, Element->Index))
                                            {
                                                GridsResult->Add(Element);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        float EuclidDist = FVector::Dist(start,end);
                        if(EuclidDist <= Dist)
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

bool GridManager::GetLargeCharacterGrids(int Index, TArray<Grid*>& Out) const
{
    if(Index < 0 || Index >= GGGrids.Num())
    {
        return false;
    }
    bool Result = true;
    Out.Empty();
    Out.Add(const_cast<Grid*>(&GGGrids[Index]));
    
    FIntPoint Point = IndexToPoint(Index);
    if((Point - FIntPoint(1,0)).X >= 0)
    {
        int LeftIndex = PointToIndex(Point - FIntPoint(1,0));
        Out.Add(const_cast<Grid*>(&GGGrids[LeftIndex]));
    }
    else
    {
        Result = false;
    }

    if((Point - FIntPoint(1,1)).X >= 0 && (Point - FIntPoint(1,1)).Y >= 0)
    {
        int BotLeftIndex = PointToIndex(Point - FIntPoint(1,1));
        Out.Add(const_cast<Grid*>(&GGGrids[BotLeftIndex]));
    }
    else
    {
        Result = false;
    }

    if((Point - FIntPoint(0,1)).Y >= 0)
    {
        int BotIndex = PointToIndex(Point - FIntPoint(0,1));
        Out.Add(const_cast<Grid*>(&GGGrids[BotIndex]));
    }
    else
    {
        Result = false;
    }

    return Result;
    
}

int GridManager::GetIntercept(TArray<Grid*>* Grids1, TArray<Grid*>* Grids2, TArray<Grid*>* GridsResult)
{
    if(Grids1 == nullptr || Grids2 == nullptr)
    {
        return 0;
    }

    int Result = 0;

    for (auto Element : *Grids1)
    {
        for (auto Element2 : *Grids2)
        {
            if(Element == Element2)
            {
                Result++;
                if(GridsResult != nullptr)
                {
                    GridsResult->Add(Element);
                }
            }
        }
    }

    return Result;
}

bool GridManager::DoesInclude(TArray<Grid*>* Grids, int Index)
{
    if(Grids == nullptr)
    {
        return false;
    }

    if(Index < 0 || Index >= ColumnCount*RowCount)
    {
        return false;
    }

    for (auto Element : *Grids)
    {
        if(Element)
        {
            if(Element == &GGGrids[Index])
            {
                return true;
            }
        }
    }

    return false;
}

int GridManager::GetClosestInArray(TArray<Grid*>* Grids, int Index) const
{
    if(Grids == nullptr)
    {
        return 0;
    }

    if(Index < 0 || Index >= ColumnCount*RowCount)
    {
        return 0;
    }

    int Result = 0;
    float MinDist = 999999999;
    float Dist = 0;

    for (int i = 0; i < Grids->Num(); i++)
    {
        Grid* Element = (*Grids)[i];
        if(Element)
        {
            Dist = GetDistBetween(Element->Index, Index);
            if(Dist < MinDist)
            {
                MinDist = Dist;
                Result = Element->Index;
            }
        }
    }

    return Result;
}

bool GridManager::GetCharactersInArray(TArray<Grid*>* Grids, TArray<AGGCharacter*>* Characters) const
{
    if(Grids == nullptr || Characters == nullptr)
    {
        return false;
    }

    if(AttachedFloor == nullptr)
    {
        return false;
    }

    AGuildGameGameModeBase* GameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(AttachedFloor));

    if(GameMode == nullptr)
    {
        return false;
    }

    bool Result = false;
    
    for (auto Grid : *Grids)
    {
        if(Grid)
        {
            for (auto Char : GameMode->GetCharacterList())
            {
                if(Char)
                {
                    if(Char->GetCurrentIndex() == Grid->Index)
                    {
                        Characters->Add(Char);
                        Result = true;
                    }
                }
            }
        }
    }
    return Result;
}

AGGCharacter* GridManager::GetCharacterByGridIndex(int GridIndex) const
{
     if(AttachedFloor == nullptr)
    {
        return nullptr;
    }

    AGuildGameGameModeBase* GameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(AttachedFloor));
    if(GameMode == nullptr)
    {
        return nullptr;
    }

    for (auto Char : GameMode->GetCharacterList())
    {
        if(Char)
        {
            if(Char->GetCurrentIndex() == GridIndex)
            {
                return  Char;
            }
        }
    }

    return nullptr;
}

bool GridManager::CanAttackTargetGrid(AGGCharacter* Character, FPredictProjectilePathResult& ProjectileResult)
{
	if (Character == nullptr)
	{
		return false;
	}
	
	return PredictAttackOnTargetGrid(Character, ProjectileResult, Character->GetStartTrajectoryLocation());
}
bool GridManager::CanAttackTargetGrid(AGGCharacter* Character, FPredictProjectilePathResult& ProjectileResult, FVector StartLocation)
{
	if (Character == nullptr)
	{
		return false;
	}
	
	return PredictAttackOnTargetGrid(Character, ProjectileResult, StartLocation);
}

bool GridManager::PredictAttackOnTargetGrid(AGGCharacter* Character, FPredictProjectilePathResult& ProjectileResult,
    FVector StartLocation)
{
    if (Character == nullptr)
	{
		return false;
	}
	
	CharacterSkill* CurrentSkill = Character->GetCurrentSkill();

	if(CurrentSkill)
	{
        FSkillData* SkillData = &(CurrentSkill->GetSkillData());

	    if(SkillData)
	    {
            ELineOfSightType SightType = SkillData->SkillLineOfSight;
	        float Angle = SkillData->LineOfSightAngle;

	        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToHit;
	        TArray<AActor*> ActorsToIgnore;
	        ActorsToIgnore.Add(Character);
		    
	        ESkillTargetingType TargetingType =  CurrentSkill->GetSkillData().TargetingType;

	        AGGCharacter* CharacterToHit = Character->GetCharacterAtTargetGridIndex();
		    
	        AGuildGameGameModeBase* GameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(Character->GetWorld()));
	        if(SightType == ELineOfSightType::GoesThroughEverything)
	        {
                if(GameMode != nullptr)
                {
                    TArray<AGGCharacter*> AllCharacters = GameMode->GetCharacterList();

                    for (int i = 0; i < AllCharacters.Num(); ++i)
                    {
                        if(AllCharacters[i] != nullptr && AllCharacters[i] != CharacterToHit && AllCharacters[i] != Character)
                        {
                            ActorsToIgnore.AddUnique(AllCharacters[i]);
                        }
                    }
                }
	            ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery1);//World_Static
	            ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery3);//Pawn
	        }
	        else if(SightType == ELineOfSightType::GoesThroughNothing)
	        {
	            ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery1);//World_Static
                ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery3);//Pawn
                ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery6);//Destructible
                ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery7);//Obstacles
	        }
	        else if(SightType == ELineOfSightType::GoesThroughCharacters)
	        {
	            if(GameMode != nullptr)
                {
                    TArray<AGGCharacter*> AllCharacters = GameMode->GetCharacterList();

                    for (int i = 0; i < AllCharacters.Num(); ++i)
                    {
                        if(AllCharacters[i] != nullptr && AllCharacters[i] != CharacterToHit && AllCharacters[i] != Character)
                        {
                            ActorsToIgnore.AddUnique(AllCharacters[i]);
                        }
                    }
                }
	            ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery1);//World_Static
                ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery3);//Pawn
                ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery6);//Destructable
                ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery7);//Obstacles
	        }


	        bool bHit = AProjectile::TraceTrajectory(StartLocation, Character->GetTargetTrajectoryLocation(), Angle, Character->GetWorld(),ProjectileResult, ActorsToIgnore, ObjectTypesToHit);

	        AActor* ActorHit = ProjectileResult.HitResult.GetActor();
	        if(ActorHit)
	        {
	            Grid& TargetGrid = GGGrids[Character->GetCurrentTargetIndex()];
	            AGridObstacle* GridObstacle = Cast<AGridObstacle>(ActorHit);
	            AGGCharacter* CharacterHit = Cast<AGGCharacter>(ActorHit);

	            if(GridObstacle || (TargetGrid.GridState == EGridState::Obstacle && CharacterToHit == nullptr))
	            {
	                 UE_LOG(LogTemp, Warning, TEXT("OBSTACLE "));
	                return false; 
	            }

	            UE_LOG(LogTemp, Warning, TEXT("HITTED ACTOR %s "), *ActorHit->GetName());

	            // GridManager* GridMan = CharacterManager::CharGridManager;
	            
	            if(TargetingType == ESkillTargetingType::Enemy)
	            {
	                if(ActorHit == CharacterToHit)
	                {
	                    return true;
	                }
	            }
	            else if(TargetingType == ESkillTargetingType::Friend)
	            {
	                if(ActorHit == CharacterToHit)
	                {
	                    return true;
	                }
	            }
	            else if(TargetingType == ESkillTargetingType::Grid)
	            {
	                if(CharacterHit != nullptr)//meaning the hit is a character 
	                {
	                    if(CharacterHit == CharacterToHit)
	                    {
	                        return true;
	                    }
	                }
	                else
	                {
	                    return true;
	                }
	            }
	            // else if(TargetingType == ESkillTargetingType::AllCharacters)
	            // {
	            //     
	            // }
	            // else if(TargetingType == ESkillTargetingType::Caster)
	            // {
	            //     
	            // }
	        }
	        // else
	        // {
	        //     // if(TargetingType == ESkillTargetingType::Grid)
	        //     // {
	        //     //     return true;
	        //     // }
	        // }
        }
	    
	}

    return false;
}

void GridManager::GetCharsInEffectSight(TArray<AGGCharacter*> CharsInArea, TArray<AGGCharacter*>& OutCharsInSight, AGGCharacter* SelectedCharacter, UWorld* World)
{
    if(World == nullptr || SelectedCharacter == nullptr) return ;

    CharacterSkill* Skill = SelectedCharacter->GetCurrentSkill();

    if(Skill)
    {
        FSkillData* SkillData = &Skill->GetSkillData();
        if(SkillData)
        {
            FCollisionQueryParams CollisionParams;
            FHitResult HitResult;
            FCollisionObjectQueryParams ObjectsToCollide;

            AGGCharacter* TargetCharacter = SelectedCharacter->GetCharacterAtTargetGridIndex();

            ELineOfSightType SightType = SkillData->EffectLineOfSight;

            for (int i = 0; i < CharsInArea.Num(); ++i)
            {
                AGGCharacter* CharInArray = CharsInArea[i];
                if(SightType == ELineOfSightType::GoesThroughEverything)
                {
                    if(CharInArray != nullptr)
                    {
                        OutCharsInSight.Add(CharInArray);
                    }
                }
                else if(SightType == ELineOfSightType::GoesThroughNothing)
                {
                    if(TargetCharacter)
                    {
                        CollisionParams.AddIgnoredActor(TargetCharacter);
                    }
                    if(CharInArray != nullptr)
                    {
                        if(CharInArray == TargetCharacter)
                        {
                             OutCharsInSight.Add(CharInArray);
                        }
                        else
                        {
                            ObjectsToCollide.AddObjectTypesToQuery(ECC_Destructible);
                            ObjectsToCollide.AddObjectTypesToQuery(ECC_GameTraceChannel3);//Obstacles
                            ObjectsToCollide.AddObjectTypesToQuery(ECC_WorldStatic);
                            ObjectsToCollide.AddObjectTypesToQuery(ECC_Pawn);
                            
                            FVector StartLocation = SelectedCharacter->GetTargetTrajectoryLocation();
                            FVector EndLocation = CharInArray->GetActorLocation();
                            StartLocation.Z = EndLocation.Z;

                            DrawDebugLine(
                                 World,
                                 StartLocation,
                                 EndLocation,
                                 FColor(255, 0, 0),
                                 false, 3, 0,
                                 12.333
                             );
                                
                            bool bDidHit = World->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectsToCollide, CollisionParams);
                            if(bDidHit)
                            {
                                AActor* HitActor = HitResult.GetActor();
                                if(HitActor)
                                {
                                    if(HitActor == CharInArray)
                                    {
                                         OutCharsInSight.Add(CharInArray);
                                    }
                                }
                            }
                        }
                    }
                }
                else if(SightType == ELineOfSightType::GoesThroughCharacters)
                {
                    if(CharInArray == TargetCharacter)
                    {
                         OutCharsInSight.Add(CharInArray);
                    }
                    
                    if(CharInArray != nullptr)
                    {
                        if(CharInArray == TargetCharacter)
                        {
                             OutCharsInSight.Add(CharInArray);
                        }
                        else
                        {
                            ObjectsToCollide.AddObjectTypesToQuery(ECC_Destructible);
                            ObjectsToCollide.AddObjectTypesToQuery(ECC_GameTraceChannel3);//Obstacles
                            ObjectsToCollide.AddObjectTypesToQuery(ECC_WorldStatic);
                            
                            FVector StartLocation = SelectedCharacter->GetTargetTrajectoryLocation();
                            FVector EndLocation = CharInArray->GetActorLocation();
                            StartLocation.Z = EndLocation.Z;

                            DrawDebugLine(
                                 World,
                                 StartLocation,
                                 EndLocation,
                                 FColor(255, 0, 0),
                                 false, 3, 0,
                                 12.333
                             );
                            bool bDidHit = World->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectsToCollide, CollisionParams);
                            if(bDidHit == false)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("GoesThroughCharacters No Hit So Add ACTOR %s "), *CharInArray->GetName());
                                 OutCharsInSight.Add(CharInArray);
                            }
                        }
                    }
                }
            }
        }
        
    }
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
