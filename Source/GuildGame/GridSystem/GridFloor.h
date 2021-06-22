// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GuildGame/Managers/GridManager.h"
#include "GuildGame/Battle/SplineActor.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "GridFloor.generated.h"

UENUM(BlueprintType)
enum class EISMType: uint8
{
	Empty,
	Movement,
	Target,
	Damage
};

USTRUCT(BlueprintType)
struct FISMDetails : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UStaticMesh* Mesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UMaterialInterface* Material;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FLinearColor Color;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Opacity;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Z;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float GlowStrength = 30;

	//UPROPERTY(EditAnywhere,BlueprintReadOnly)
	//int StencilValue;
};

UCLASS()
class GUILDGAME_API AGridFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridFloor();
	~AGridFloor();
	

	UPROPERTY(EditAnywhere)
	int RowCount;
	UPROPERTY(EditAnywhere)
	int ColumnCount;

	UPROPERTY(EditAnywhere)
	float GridSize;
	UPROPERTY(EditAnywhere)
	float LineSize;


	UPROPERTY(EditAnywhere)
	FLinearColor LineColor;
	UPROPERTY(EditAnywhere)
	FLinearColor SelectedGridColor;

	UPROPERTY(EditAnywhere)
	float LineGlowStrength = 30;

	UPROPERTY(EditAnywhere)
	float SelectedGridGlowStrength = 30;
	
	UPROPERTY(EditAnywhere)
	float LineOpacity;
	UPROPERTY(EditAnywhere)
	float GridOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* SelectionMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProceduralMeshComponent* GridMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProceduralMeshComponent* MovementProcMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProceduralMeshComponent* TargetProcMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProceduralMeshComponent* DamageProcMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* LineMaterial;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* SelectedGridMaterial;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<EISMType,FISMDetails> ISMMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<ESplineMeshType, FSplineMeshDetails> PathSplineMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<ESplineMeshType, FSplineMeshDetails> TrajectoryTrueSplineMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<ESplineMeshType, FSplineMeshDetails> TrajectoryFalseSplineMap;

	UPROPERTY()
	USceneComponent* CustomRootComponent;
private:
	UPROPERTY(EditAnywhere)
	int GridFloorTypeCount = 3;
	
	GridManager* FloorGridManager;
	UPROPERTY()
	ASplineActor* PathActor = nullptr;
	UPROPERTY()
	ASplineActor* TrajectorySplineActor = nullptr;

	UMaterialInstanceDynamic* SelectionGridMatInst;
	
	UMaterialInstanceDynamic* MovementGridMatInst;
	UMaterialInstanceDynamic* TargetGridMatInst;
	UMaterialInstanceDynamic* DamageGridMatInst;

	void UpdateGridStatesWithTrace();

	TArray<Grid*> PlaceableGrids;

public:
	
	GridManager* GetGridManager() const;
	void SetGridManager(GridManager* AGridManager);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UMaterialInstanceDynamic* CreateMaterialInstance(FLinearColor Color, float Opacity);

	void CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles, TArray<FVector2D>* = nullptr);

	virtual void OnConstruction(const FTransform& Transform) override;

	void UpdateSelectedGrid(FVector NewPos, bool IsVisible, int Scale = 1);

	void SetSelectedGridColorType(EISMType);

	void DrawPath(int StartIndex, int EndIndex, bool LargeGrid = false);

	float GetPathLength(int StartIndex, int EndIndex);

	bool UpdateGridMeshes(TArray<Grid*>& GridsToUpdate, EISMType = EISMType::Movement, bool ClearAll = true, TArray<Grid*>* ExcludeList = nullptr);

	void ClearGridMeshes();

	void ClearGridMesh(EISMType);

	void ClearPath();

	void ConstructProcMeshes();

	void CreateProceduralGridArea(EISMType Type, TArray<Grid*>& Grids, TArray<Grid*>* ExcludeList = nullptr);

	void SetProcMaterials(EISMType Type);

	void SetProcMeshPosition(EISMType Type, FVector&);

	void ShowPlaceableGrids();

	TArray<Grid*>& GetPlaceableGrids()
	{
		return PlaceableGrids;
	}

	//void CreatePath(int StartIndex, int EndIndex);

	void DrawTrajectory(AGGCharacter* Character);
	void ClearTrajectory();
};
