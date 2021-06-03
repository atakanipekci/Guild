// Fill out your copyright notice in the Description page of Project Settings.


#include "GridFloor.h"


#include "DrawDebugHelpers.h"
#include "GGLogHelper.h"
#include "GuildGame/Managers/CharacterManager.h"
#include "Kismet/GameplayStatics.h"
#include "GuildGame/Battle/BattlePlayerController.h"
#include "GuildGame/GridSystem/Navigation/GridNavigationData.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GuildGame/Battle/SplineActor.h"

AGridFloor::AGridFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(CustomRootComponent);
	SelectionMesh = CreateDefaultSubobject<UProceduralMeshComponent>("SelectionMesh");
	SelectionMesh->SetupAttachment(CustomRootComponent);
	MovementProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("MovementProcMesh");
	MovementProcMesh->SetupAttachment(CustomRootComponent);
	TargetProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("TargetProcMesh");
	TargetProcMesh->SetupAttachment(CustomRootComponent);
	DamageProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("DamageProcMesh");
	DamageProcMesh->SetupAttachment(CustomRootComponent);
	GridMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	GridMesh->SetupAttachment(CustomRootComponent);
	SelectionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MovementProcMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TargetProcMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageProcMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GridMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

AGridFloor::~AGridFloor()
{
	/*if(MovementProcMesh)
	{
		MovementProcMesh->ClearAllMeshSections();
		//MovementProcMesh->DestroyComponent();
	}
	if(TargetProcMesh)
	{
		TargetProcMesh->ClearAllMeshSections();
		//TargetProcMesh->DestroyComponent();
	}
	if(DamageProcMesh)
	{
		DamageProcMesh->ClearAllMeshSections();
		//DamageProcMesh->DestroyComponent();
	}*/
	delete FloorGridManager;
}

void AGridFloor::UpdateGridStatesWithTrace()
{
	FCollisionShape MySphereCollision = FCollisionShape::MakeSphere(GridSize/2 - GridSize/10);
	FHitResult OutHit;
	for(int i = 0; i < ColumnCount; i++)
	{
		for(int j = 0; j < RowCount; j++)
		{
			FVector pos = FloorGridManager->GetGridCenter(i + j*ColumnCount);
			pos.Z = GetActorLocation().Z;
			//ecc_gametracechannel1 should be gridobstacle channel.
			bool isHit = GetWorld()->SweepSingleByChannel(OutHit,pos,pos,FQuat::Identity,ECC_GameTraceChannel1,MySphereCollision);
			if(isHit)
			{
				DrawDebugSphere(GetWorld(), pos, MySphereCollision.GetSphereRadius(), 12, FColor::Purple, true);
				FloorGridManager->SetGridState(i + j*ColumnCount, EGridState::Obstacle);
			}
		}
	}
}


GridManager* AGridFloor::GetGridManager() const
{
	return FloorGridManager;
}

void AGridFloor::SetGridManager(GridManager* NewGridManager)
{
	this->FloorGridManager = NewGridManager;
}

// Called when the game starts or when spawned
void AGridFloor::BeginPlay()
{
	Super::BeginPlay();
	ABattlePlayerController* PlayerController = Cast<ABattlePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "setset");
		PlayerController->SetGridFloor(this);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "setfail");
	}

	//LOG("col count %d", ColumnCount);
	FVector2D StartPos = FVector2D(this->GetActorLocation().X,this->GetActorLocation().Y);
	FloorGridManager = new GridManager(StartPos,this->GridSize,this->ColumnCount, this->RowCount);
	FloorGridManager->SetAttachedFloor(this);
	CharacterManager::CharGridManager = FloorGridManager;
	

	/*FVector TempPos = FloorGridManager->GetGridCenter(10);
	TempPos.Z = GetActorLocation().Z;
	AvailableGridMesh->AddInstanceWorldSpace(FTransform{TempPos});
	TempPos = FloorGridManager->GetGridCenter(11);
	TempPos.Z = GetActorLocation().Z;
	TargetGridMesh->AddInstanceWorldSpace(FTransform{TempPos});*/

	UpdateGridStatesWithTrace();
	
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		PathActor = static_cast<ASplineActor*>(World->SpawnActor<ASplineActor>(FVector{0,0,0},FRotator{},SpawnParams));
		if(PathActor)
		{
			PathActor->ClearNodes();

			PathActor->SplineMeshMap = PathSplineMap;
			
			/*for(int i = 0; i < 5; i++)
			{
				PathActor->AddNode(FVector{static_cast<float>(i*200),0,0});
			}*/
			PathActor->UpdateSpline();
			PathActor->SetActorScale3D(FVector{0.05,0.05,0.001});
		}

		FActorSpawnParameters SpawnParams2;
		TrajectorySplineActor = static_cast<ASplineActor*>(World->SpawnActor<ASplineActor>(FVector{0,0,0},FRotator{},SpawnParams2));

		if(TrajectorySplineActor)
		{
			TrajectorySplineActor->SetActorScale3D(FVector{0.1,0.1,0.1});
			TrajectorySplineActor->ClearNodes();
			// TrajectorySplineActor->SplineMeshMap = TrajectorySplineMap;
			TrajectorySplineActor->UpdateSpline();
		}

	}
	
}

// Called every frame
void AGridFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UMaterialInstanceDynamic* AGridFloor::CreateMaterialInstance(FLinearColor Color, float Opacity)
{
	if(GridMesh==nullptr)
	{
		return nullptr;
	}

	UMaterialInstanceDynamic* GridMat = GridMesh->CreateAndSetMaterialInstanceDynamic(0);
	if(GridMat != nullptr)
	{
		GridMat->SetVectorParameterValue(FName("Color"), Color);
		GridMat->SetVectorParameterValue(FName("Glow Color"), Color);
		GridMat->SetScalarParameterValue(FName("Opacity"), Opacity);
		GridMat->SetScalarParameterValue(FName("Glow Strength"), LineGlowStrength);
		//GridMat->SetScalarParameterValue(FName("Edge Sharpness"), 30);
		//GridMat->SetScalarParameterValue(FName("Edge Subtraction"), 0.1);
	}

	return GridMat;

	
	
}

void AGridFloor::CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices,
	TArray<int>& Triangles, TArray<FVector2D>* UVs)
{
	float HalfThickness = Thickness/2;

	FVector ThicknessDirection = FVector(End - Start);
	ThicknessDirection.Normalize(0.0001);
	ThicknessDirection = ThicknessDirection.CrossProduct(ThicknessDirection,FVector(0,0,1));

	Triangles.Add(Vertices.Num() + 2 );
	Triangles.Add(Vertices.Num() + 1 );
	Triangles.Add(Vertices.Num() + 0 );
	Triangles.Add(Vertices.Num() + 2 );
	Triangles.Add(Vertices.Num() + 3 );
	Triangles.Add(Vertices.Num() + 1 );

	Vertices.Add(Start + ThicknessDirection*HalfThickness);
	Vertices.Add(End + ThicknessDirection*HalfThickness);
	Vertices.Add(Start - ThicknessDirection*HalfThickness);
	Vertices.Add(End - ThicknessDirection*HalfThickness);
	if(UVs)
	{
		UVs->Add(FVector2D(0,0));
		UVs->Add(FVector2D(0,1));
		UVs->Add(FVector2D(1,0));
		UVs->Add(FVector2D(1,1));
	}
}

void AGridFloor::OnConstruction(const FTransform& Transform)
{
	if(GridMesh)
	{
		GridMesh->SetMaterial(0, LineMaterial);
	}

	if(SelectedGridMaterial)
	{
		if(SelectionMesh)
			SelectionMesh->SetMaterial(0, SelectedGridMaterial);
	}

	ConstructProcMeshes();
	
	UMaterialInstanceDynamic* LineMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	SelectionGridMatInst = SelectionMesh->CreateAndSetMaterialInstanceDynamic(0);

	if(SelectionGridMatInst != nullptr)
	{
		SelectionGridMatInst->SetVectorParameterValue(FName("Color"), SelectedGridColor);
		SelectionGridMatInst->SetVectorParameterValue(FName("Glow Color"), SelectedGridColor);
		SelectionGridMatInst->SetScalarParameterValue(FName("Opacity"), GridOpacity);
		SelectionGridMatInst->SetScalarParameterValue(FName("Glow Strength"), SelectedGridGlowStrength);
		//SelectionGridMatInst->SetScalarParameterValue(FName("Edge Sharpness"), 30);
		//SelectionGridMatInst->SetScalarParameterValue(FName("Edge Subtraction"), 0.1);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "NULL MAT");
	}

	TArray<FVector> LineVertices;
	TArray<int> LineTriangles;
	TArray<FVector2D> UVs;
	for(int i = 0; i <= RowCount; i++)
	{
		float LineStart = i*GridSize;
		float LineEnd = ColumnCount*GridSize;
		CreateLine(FVector(LineStart,0,0),FVector(LineStart,LineEnd,0),LineSize,LineVertices, LineTriangles);
	}

	for(int i = 0; i <= ColumnCount; i++)
	{
		float LineStart = i*GridSize;
		float LineEnd = RowCount*GridSize;
		CreateLine(FVector(0,LineStart,0),FVector(LineEnd,LineStart,0),LineSize,LineVertices, LineTriangles);
	}
	if(GridMesh)
	{
		GridMesh->CreateMeshSection_LinearColor(0,LineVertices,LineTriangles,TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(),false);
	}

	TArray<FVector> SelectionVertices;
	TArray<int> SelectionTriangles;
	CreateLine(FVector(0,GridSize/2,0), FVector(GridSize,GridSize/2,0),GridSize,SelectionVertices, SelectionTriangles, &UVs);
	SelectionMesh->CreateMeshSection_LinearColor(0,SelectionVertices,SelectionTriangles,TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(),false);
	SelectionMesh->SetVisibility(false);
}

void AGridFloor::UpdateSelectedGrid(FVector NewPos, bool IsVisible)
{
	if(SelectionMesh == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "null");
		return;
	}

	SelectionMesh->SetWorldLocation(FVector(NewPos.X, NewPos.Y, GetActorLocation().Z));
	SelectionMesh->SetVisibility(IsVisible);
	//SelectionMesh->SetRenderCustomDepth(true);
	
}

void AGridFloor::DrawPath(int StartIndex, int EndIndex)
{
	if(!FloorGridManager || !PathActor)
	{
		return;
	}

	PathActor->ClearNodes();
	PathActor->UpdateSpline();
	if(FloorGridManager->IsGridValid(StartIndex) && FloorGridManager->IsGridValid(EndIndex))
	{
		FVector start = FloorGridManager->GetGridCenter(StartIndex);
		FVector end = FloorGridManager->GetGridCenter(EndIndex);
		UNavigationPath* path =  UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), start, end, nullptr,nullptr);
		if(path && path->IsValid())
		{
				//LOG("path->PathPoints.Num() %d", path->PathPoints.Num());
			for(int i = 0; i < path->PathPoints.Num(); i++)
			{
				FVector aa = path->PathPoints[i];
				aa.Z = 70;
				PathActor->AddNode(aa);
				//LOG("i : %d", FloorGridManager->WorldToGrid(path->PathPoints[i]));
			}
			PathActor->UpdateSpline();
		}
	}
}

float AGridFloor::GetPathLength(int StartIndex, int EndIndex)
{
	FVector start = FloorGridManager->GetGridCenter(StartIndex);
	FVector end = FloorGridManager->GetGridCenter(EndIndex);
	UNavigationPath* path =  UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), start, end, nullptr,nullptr);
	if(path && path->IsValid())
	{
		return path->GetPathLength();
	}
	return -1;
}

bool AGridFloor::UpdateGridMeshes(TArray<Grid*>& GridsToUpdate, EISMType MeshType, bool ClearAll, TArray<Grid*>* ExcludeList)
{
	if(ClearAll)
	{
		ClearGridMeshes();	
	}
	
	CreateProceduralGridArea(MeshType,GridsToUpdate, ExcludeList);

	return true;
}

void AGridFloor::ClearGridMeshes()
{

	for(int i = 0; i < GridFloorTypeCount; i++)
	{
		ClearGridMesh(static_cast<EISMType>(i));
	}
}

void AGridFloor::ClearGridMesh(EISMType Type)
{
	switch (Type)
	{
		case EISMType::Movement:
			//DamageGridMesh->ClearInstances();
			if(MovementProcMesh)
			{
				MovementProcMesh->ClearAllMeshSections();
				MovementProcMesh->SetRelativeLocation(FVector::ZeroVector);
			}
			break;

		case EISMType::Target:
			//MovementGridMesh->ClearInstances();
			if(TargetProcMesh)
			{
				TargetProcMesh->ClearAllMeshSections();
				TargetProcMesh->SetRelativeLocation(FVector::ZeroVector);
			}
			break;

		case EISMType::Damage:
			//TargetGridMesh->ClearInstances();
			if(DamageProcMesh)
			{
				DamageProcMesh->ClearAllMeshSections();
				DamageProcMesh->SetRelativeLocation(FVector::ZeroVector);
			}
			break;

		default:
			break;
	}
}

void AGridFloor::ClearPath()
{
	if(PathActor == nullptr)
	{
		return;
	}
	PathActor->ClearNodes();
	PathActor->UpdateSpline();
}

void AGridFloor::ConstructProcMeshes()
{
	if(SelectedGridMaterial)
	{
		if(MovementProcMesh)
			MovementProcMesh->SetMaterial(0, SelectedGridMaterial);

		if(TargetProcMesh)
			TargetProcMesh->SetMaterial(0, SelectedGridMaterial);

		if(DamageProcMesh)
			DamageProcMesh->SetMaterial(0, SelectedGridMaterial);
	}

	
	for(int i = 0; i < GridFloorTypeCount; i++)
	{
		switch(i)
		{
			case EISMType::Movement:
				SetProcMaterials(EISMType::Movement);
				break;

			case EISMType::Target:
				SetProcMaterials(EISMType::Target);
				break;

			case EISMType::Damage:
				SetProcMaterials(EISMType::Damage);
				break;

			default:
				break;
		}
	}
}

void AGridFloor::SetProcMaterials(EISMType Type)
{
	UProceduralMeshComponent* Mesh = nullptr;
	switch (Type)
	{
		case EISMType::Movement:
			Mesh = MovementProcMesh;
			/*if(Mesh)
				Mesh->SetCustomDepthStencilValue(ISMMap.Find(Type)->StencilValue);*/
			break;

		case EISMType::Target:
			Mesh = TargetProcMesh;
			/*if(Mesh)
				Mesh->SetCustomDepthStencilValue(ISMMap.Find(Type)->StencilValue);*/
			break;

		case EISMType::Damage:
			Mesh = DamageProcMesh;
			/*if(Mesh)
				Mesh->SetCustomDepthStencilValue(ISMMap.Find(Type)->StencilValue);*/
			break;

		default:
			break;
	}
	
	if(Mesh == nullptr)
	{
		return;
	}

	if(Mesh)
	{
		UMaterialInstanceDynamic* MaterialInstance =Mesh->CreateAndSetMaterialInstanceDynamic(0);

		if(MaterialInstance != nullptr)
		{
			MaterialInstance->SetVectorParameterValue(FName("Color"), ISMMap.Find(Type)->Color);
			MaterialInstance->SetVectorParameterValue(FName("Glow Color"), ISMMap.Find(Type)->Color);
			MaterialInstance->SetScalarParameterValue(FName("Opacity"), ISMMap.Find(Type)->Opacity);
			MaterialInstance->SetScalarParameterValue(FName("Glow Strength"), ISMMap.Find(Type)->GlowStrength);
			//MaterialInstance->SetScalarParameterValue(FName("Edge Sharpness"), 10);
			//MaterialInstance->SetScalarParameterValue(FName("Edge Subtraction"), 0.1);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "NULL MAT");
		}
		//Mesh->SetRenderCustomDepth(true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "NULL MESH");
	}
	
	
}

void AGridFloor::SetProcMeshPosition(EISMType Type, FVector& AddPosition)
{
	UProceduralMeshComponent* ProcMesh = nullptr;
	switch (Type)
	{
		case EISMType::Movement:
			ProcMesh = MovementProcMesh;
			break;

		case EISMType::Target:
			ProcMesh = TargetProcMesh;
			break;

		case EISMType::Damage:
			ProcMesh = DamageProcMesh;
			break;

		default:
			ProcMesh = MovementProcMesh;
			break;
	}

	if(ProcMesh == nullptr)
	{
		return;
	}

	ProcMesh->SetRelativeLocation(ProcMesh->GetRelativeLocation()+AddPosition);
	
}

void AGridFloor::CreateProceduralGridArea(EISMType Type, TArray<Grid*>& Grids, TArray<Grid*>* ExcludeList)
{
	if(GetGridManager() == nullptr)
	{
		return;
	}
	UProceduralMeshComponent* ProcMesh = nullptr;
	UMaterialInstanceDynamic* MatInstance = nullptr;
	switch (Type)
	{
		case EISMType::Movement:
			ProcMesh = MovementProcMesh;
			if(MovementGridMatInst == nullptr && ProcMesh)
			{
				MovementGridMatInst = ProcMesh->CreateAndSetMaterialInstanceDynamic(0);
				MovementGridMatInst->SetVectorParameterValue(FName("Color"), ISMMap.Find(Type)->Color);
				MovementGridMatInst->SetVectorParameterValue(FName("Glow Color"), ISMMap.Find(Type)->Color);
				MovementGridMatInst->SetScalarParameterValue(FName("Opacity"), ISMMap.Find(Type)->Opacity);
				MovementGridMatInst->SetScalarParameterValue(FName("Glow Strength"), ISMMap.Find(Type)->GlowStrength);
			}
			MatInstance = MovementGridMatInst;
			/*if(ProcMesh)
				ProcMesh->SetCustomDepthStencilValue(ISMMap.Find(Type)->StencilValue);*/
			break;

		case EISMType::Target:
			ProcMesh = TargetProcMesh;
			if(TargetGridMatInst == nullptr && ProcMesh)
			{
				TargetGridMatInst = ProcMesh->CreateAndSetMaterialInstanceDynamic(0);
				TargetGridMatInst->SetVectorParameterValue(FName("Color"), ISMMap.Find(Type)->Color);
				TargetGridMatInst->SetVectorParameterValue(FName("Glow Color"), ISMMap.Find(Type)->Color);
				TargetGridMatInst->SetScalarParameterValue(FName("Opacity"), ISMMap.Find(Type)->Opacity);
				TargetGridMatInst->SetScalarParameterValue(FName("Glow Strength"), ISMMap.Find(Type)->GlowStrength);
			}
			MatInstance = TargetGridMatInst;
			/*if(ProcMesh)
				ProcMesh->SetCustomDepthStencilValue(ISMMap.Find(Type)->StencilValue);*/
			break;

		case EISMType::Damage:
			ProcMesh = DamageProcMesh;
			if(DamageGridMatInst == nullptr && ProcMesh)
			{
				DamageGridMatInst = ProcMesh->CreateAndSetMaterialInstanceDynamic(0);
				DamageGridMatInst->SetVectorParameterValue(FName("Color"), ISMMap.Find(Type)->Color);
				DamageGridMatInst->SetVectorParameterValue(FName("Glow Color"), ISMMap.Find(Type)->Color);
				DamageGridMatInst->SetScalarParameterValue(FName("Opacity"), ISMMap.Find(Type)->Opacity);
				DamageGridMatInst->SetScalarParameterValue(FName("Glow Strength"), ISMMap.Find(Type)->GlowStrength);
			}
			MatInstance = DamageGridMatInst;
			/*if(ProcMesh)
				ProcMesh->SetCustomDepthStencilValue(ISMMap.Find(Type)->StencilValue);*/
			break;

		default:
			ProcMesh = MovementProcMesh;
			if(MovementGridMatInst == nullptr && ProcMesh)
			{
				MovementGridMatInst = ProcMesh->CreateAndSetMaterialInstanceDynamic(0);
				MovementGridMatInst->SetVectorParameterValue(FName("Color"), ISMMap.Find(Type)->Color);
				MovementGridMatInst->SetVectorParameterValue(FName("Glow Color"), ISMMap.Find(Type)->Color);
				MovementGridMatInst->SetScalarParameterValue(FName("Opacity"), ISMMap.Find(Type)->Opacity);
				MovementGridMatInst->SetScalarParameterValue(FName("Glow Strength"), ISMMap.Find(Type)->GlowStrength);
			}
			MatInstance = MovementGridMatInst;
			/*if(ProcMesh)
				ProcMesh->SetCustomDepthStencilValue(ISMMap.Find(Type)->StencilValue);*/
			break;
	}

	if(ProcMesh == nullptr)
	{
		return;
	}

	ProcMesh->ClearAllMeshSections();
	
	TArray<FVector> LineVertices;
	TArray<int> LineTriangles;
	TArray<FVector2D> UVs;

	TArray<Grid*> InterceptingGrids;
	
	if(ExcludeList)
	{
		FloorGridManager->GetIntercept(&Grids, ExcludeList, &InterceptingGrids);
	}

	for (auto Element : Grids)
	{
		if(Element)
		{
			bool flag = false;
			for (auto Intercept : InterceptingGrids)
			{
				if(Intercept == Element)
				{
					flag = true;
					break;
				}
			}

			if(flag)
			{
				continue;
			}
			
			FVector Start = FloorGridManager->GetGridLeftMid(Element->Index) - this->GetActorLocation();
			Start.Z = this->GetActorLocation().Z + ISMMap.Find(Type)->Z;
			FVector End = FloorGridManager->GetGridRightMid(Element->Index)  - this->GetActorLocation();
			End.Z = this->GetActorLocation().Z + ISMMap.Find(Type)->Z;
			CreateLine(Start,End,GridSize,LineVertices, LineTriangles, &UVs);
		}
	}

	ProcMesh->CreateMeshSection_LinearColor(0,LineVertices,LineTriangles,TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(),false);
	ProcMesh->SetVisibility(true);

	//ProcMesh->SetRenderCustomDepth(true);
	//ProcMesh->SetCustomDepthStencilValue(2);

	if(SelectionMesh)
		SelectionMesh->SetCustomDepthStencilValue(ProcMesh->CustomDepthStencilValue);


	
	if(SelectionGridMatInst)
	{
		SelectionGridMatInst->SetVectorParameterValue(FName("Color"), ISMMap.Find(Type)->Color);
		SelectionGridMatInst->SetVectorParameterValue(FName("Glow Color"), ISMMap.Find(Type)->Color);
	}
	
	
}

void AGridFloor::DrawTrajectory(AGGCharacter* Character)
{
	if (Character == nullptr)
	{
		return;
	}
	
	FPredictProjectilePathResult ProjectileResult;

	bool bCanAttacked = GridManager::CanAttackTargetGrid(Character, ProjectileResult);

	if(bCanAttacked)
	{
		TrajectorySplineActor->SplineMeshMap = TrajectoryTrueSplineMap;
	}
	else
	{
		TrajectorySplineActor->SplineMeshMap = TrajectoryFalseSplineMap;
	}
	
	if(TrajectorySplineActor)
	{
		TrajectorySplineActor->ClearNodes();
		for(int i = 0; i < ProjectileResult.PathData.Num(); i++)
		{
			TrajectorySplineActor->AddNode(ProjectileResult.PathData[i].Location);
		}
		TrajectorySplineActor->UpdateSpline();
	}
}

void AGridFloor::ClearTrajectory()
{
	if(TrajectorySplineActor == nullptr)
	{
		return;
	}
	TrajectorySplineActor->ClearNodes();
	TrajectorySplineActor->UpdateSpline();
}



