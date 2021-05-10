// Fill out your copyright notice in the Description page of Project Settings.


#include "GridFloor.h"


#include "DrawDebugHelpers.h"
#include "GGLogHelper.h"
#include "GuildGame/Managers/CharacterManager.h"
#include "Kismet/GameplayStatics.h"
#include "GuildGame/Battle/GGPlayerController.h"
#include "GuildGame/GridSystem/Navigation/GridNavigationData.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GuildGame/Battle/SplineActor.h"

AGridFloor::AGridFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootComponent);
	SelectionMesh = CreateDefaultSubobject<UProceduralMeshComponent>("SelectionMesh");
	SelectionMesh->SetupAttachment(RootComponent);
	GridMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	GridMesh->SetupAttachment(RootComponent);
	MovementGridMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("AvailableGrids");
	MovementGridMesh->SetupAttachment(RootComponent);
	AttackGridMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("NotAvailableGrids");
	AttackGridMesh->SetupAttachment(RootComponent);
	MovementGridMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackGridMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SelectionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GridMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

AGridFloor::~AGridFloor()
{
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
			//DrawDebugSphere(GetWorld(), pos, MySphereCollision.GetSphereRadius(), 1, FColor::Purple, true);
			//ecc_gametracechannel1 should be gridobstacle channel.
			bool isHit = GetWorld()->SweepSingleByChannel(OutHit,pos,pos,FQuat::Identity,ECC_GameTraceChannel1,MySphereCollision);
			if(isHit)
			{
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
	AGGPlayerController* PlayerController = Cast<AGGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
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
	AttackGridMesh->AddInstanceWorldSpace(FTransform{TempPos});*/

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
			PathActor->SetActorScale3D(FVector{0.25,0.25,0.1});
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
		GridMat->SetScalarParameterValue(FName("Opacity"), Opacity);
	}

	return GridMat;

	
	
}

void AGridFloor::CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices,
	TArray<int>& Triangles)
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
}

void AGridFloor::OnConstruction(const FTransform& Transform)
{
	if(GridMesh)
	{
		GridMesh->SetMaterial(0, LineMaterial);
	}

	if(SelectedGridMaterial)
	{
		SelectionMesh->SetMaterial(0, SelectedGridMaterial);
	}
	
	UMaterialInstanceDynamic* LineMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	UMaterialInstanceDynamic* GridMaterialInstance =SelectionMesh->CreateAndSetMaterialInstanceDynamic(0);

	if(GridMaterialInstance != nullptr)
	{
		GridMaterialInstance->SetVectorParameterValue(FName("Color"), SelectedGridColor);
		GridMaterialInstance->SetScalarParameterValue(FName("Opacity"), GridOpacity);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "NULL MAT");
	}

	TArray<FVector> LineVertices;
	TArray<int> LineTriangles;
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

	GridMesh->CreateMeshSection_LinearColor(0,LineVertices,LineTriangles,TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(),false);

	TArray<FVector> SelectionVertices;
	TArray<int> SelectionTriangles;
	CreateLine(FVector(0,GridSize/2,0), FVector(GridSize,GridSize/2,0),GridSize,SelectionVertices, SelectionTriangles);
	SelectionMesh->CreateMeshSection_LinearColor(0,SelectionVertices,SelectionTriangles,TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(),false);
	SelectionMesh->SetVisibility(false);

	UInstancedStaticMeshComponent* ISMPointer;
	FISMDetails* ISMDetails;
	for(int i = 0; i < ISMMap.Num(); i++ )
	{
		switch (i)
		{
			case 0:
				ISMDetails = ISMMap.Find(EISMType::Movement);
				ISMPointer = MovementGridMesh;
			break;
				
			case 1:
				ISMDetails = ISMMap.Find(EISMType::Attack);
				ISMPointer = AttackGridMesh;
			break;

			default:
				ISMDetails = ISMMap.Find(EISMType::Movement);
				ISMPointer = AttackGridMesh;
			break;
		}

		if(ISMPointer == nullptr || ISMDetails == nullptr || ISMDetails->Mesh == nullptr || ISMDetails->Material == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "NULL ISM");
			return;
		}

		ISMPointer->SetStaticMesh(ISMDetails->Mesh);
		ISMPointer->ClearInstances();
		ISMPointer->SetMaterial(0, ISMDetails->Material);
		UMaterialInstanceDynamic* InstancedMaterial =ISMPointer->CreateAndSetMaterialInstanceDynamic(0);
		if(InstancedMaterial != nullptr)
		{
			InstancedMaterial->SetVectorParameterValue(FName("Color"), ISMDetails->Color);
			InstancedMaterial->SetScalarParameterValue(FName("Opacity"), ISMDetails->Opacity);
		}		
	}
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
				aa.Z = 50;
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

bool AGridFloor::UpdateGridMeshes(TArray<GGGrid*>& GridsToUpdate, EISMType MeshType) const
{

	for(int i = 0; i < GridFloorTypeCount; i++)
	{
		switch (i)
		{
			case EISMType::Movement:
				if(MovementGridMesh)
				{
					MovementGridMesh->ClearInstances();
				}
			break;
					
			case EISMType::Attack:
				if(AttackGridMesh)
				{
					AttackGridMesh->ClearInstances();
				}
			break;

			default:
			break;
		}
	}
	FVector Pos;
	for(int i = 0; i < GridsToUpdate.Num(); i++)
	{
		Pos.Set(FloorGridManager->GetGridCenter(GridsToUpdate[i]->Index).X,FloorGridManager->GetGridCenter(GridsToUpdate[i]->Index).Y, GetActorLocation().Z - 5);
		switch(MeshType)
		{
			case EISMType::Movement:
			if(MovementGridMesh)
			{
				MovementGridMesh->AddInstanceWorldSpace(FTransform{Pos});
			}
			break;

			case EISMType::Attack:
			if(AttackGridMesh)
			{
				AttackGridMesh->AddInstanceWorldSpace(FTransform{Pos});
			}
			break;

			default:
				MovementGridMesh->AddInstanceWorldSpace(FTransform{Pos});
			break;
		}
	}

	return true;
}

void AGridFloor::ClearGridMeshes()
{
	if(MovementGridMesh)
	{
		MovementGridMesh->ClearInstances();
	}
		
	if(AttackGridMesh)
	{
		AttackGridMesh->ClearInstances();
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



