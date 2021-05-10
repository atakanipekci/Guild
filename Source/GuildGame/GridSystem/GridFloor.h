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
	Movement,
	Attack
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
	float LineOpacity;
	UPROPERTY(EditAnywhere)
	float GridOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* SelectionMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProceduralMeshComponent* GridMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* LineMaterial;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* SelectedGridMaterial;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UInstancedStaticMeshComponent* MovementGridMesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UInstancedStaticMeshComponent* AttackGridMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<EISMType,FISMDetails> ISMMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<ESplineMeshType, FSplineMeshDetails> PathSplineMap;

	USceneComponent* RootComponent;
private:
	UPROPERTY(EditAnywhere)
	int GridFloorTypeCount = 2;
	
	GridManager* FloorGridManager;
	ASplineActor* PathActor = nullptr;

	void UpdateGridStatesWithTrace();

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

	void CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles);

	virtual void OnConstruction(const FTransform& Transform) override;

	void UpdateSelectedGrid(FVector NewPos, bool IsVisible);

	void DrawPath(int StartIndex, int EndIndex);

	float GetPathLength(int StartIndex, int EndIndex);

	bool UpdateGridMeshes(TArray<GGGrid*>& GridsToUpdate, EISMType = EISMType::Movement) const;

	void ClearGridMeshes();

	void ClearPath();

	//void CreatePath(int StartIndex, int EndIndex);
};
