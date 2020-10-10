// Fill out your copyright notice in the Description page of Project Settings.


#include "GridFloor.h"


#include "DrawDebugHelpers.h"
#include "GGLogHelper.h"
#include "Kismet/GameplayStatics.h"
#include "GGPlayerController.h"

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
	AvailableGridMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("AvailableGrids");
	AvailableGridMesh->SetupAttachment(RootComponent);
	NotAvailableGridMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("NotAvailableGrids");
	NotAvailableGridMesh->SetupAttachment(RootComponent);
}

void AGridFloor::UpdateGridStatesWithTrace()
{
	FCollisionShape MySphereCollision = FCollisionShape::MakeSphere(GridSize/2);
	TArray<FHitResult> OutHits;
	for(int i = 0; i < ColumnCount; i++)
	{
		for(int j = 0; j < RowCount; j++)
		{
			FVector pos = FloorGridManager->GetGridCenter(i + j*ColumnCount);
			pos.Z = GetActorLocation().Z;
			//DrawDebugSphere(GetWorld(), pos, MySphereCollision.GetSphereRadius(), 1, FColor::Purple, true);
			//ecc_gametracechannel1 should be gridobstacle channel.
			bool isHit = GetWorld()->SweepMultiByChannel(OutHits,pos,pos,FQuat::Identity,ECC_GameTraceChannel1,MySphereCollision);
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
	FVector2D StartPos = FVector2D(this->GetActorLocation().X,this->GetActorLocation().Y);
	//StartPos.X += this->RowCount * this->GridSize;
	FloorGridManager = new GridManager(StartPos,this->GridSize,this->ColumnCount, this->RowCount);
	FloorGridManager->SetAttachedFloor(this);
	CharacterManager::CharGridManager = FloorGridManager;

	FVector TempPos = FloorGridManager->GetGridCenter(10);
	TempPos.Z = GetActorLocation().Z;
	AvailableGridMesh->AddInstanceWorldSpace(FTransform{TempPos});
	TempPos = FloorGridManager->GetGridCenter(11);
	TempPos.Z = GetActorLocation().Z;
	NotAvailableGridMesh->AddInstanceWorldSpace(FTransform{TempPos});

	UpdateGridStatesWithTrace();
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

	int i = 0;
	TArray<FVector> LineVertices;
	TArray<int> LineTriangles;
	for(i = 0; i <= RowCount; i++)
	{
		float LineStart = i*GridSize;
		float LineEnd = ColumnCount*GridSize;
		CreateLine(FVector(LineStart,0,0),FVector(LineStart,LineEnd,0),LineSize,LineVertices, LineTriangles);
	}

	for(i = 0; i <= ColumnCount; i++)
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
	
	for(i = 0; i < GridFloorTypeCount; i++)
	{
		switch (i)
		{
			case 0:
				//AvailableGridMesh = NewObject<UInstancedStaticMeshComponent>(this);
				//AvailableGridMesh->RegisterComponent();
				ISMPointer = AvailableGridMesh;
			break;
				
			case 1:
				//NotAvailableGridMesh = NewObject<UInstancedStaticMeshComponent>(this);
				//NotAvailableGridMesh->RegisterComponent();
				ISMPointer = NotAvailableGridMesh;
			break;

			default:
				//NotAvailableGridMesh = NewObject<UInstancedStaticMeshComponent>(this);
				//NotAvailableGridMesh->RegisterComponent();
				ISMPointer = NotAvailableGridMesh;
			break;
		}

		if(ISMPointer == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "NULL ISM");
		}

		if(GridStaticMesh && ISMPointer)
		{
			ISMPointer->SetStaticMesh(GridStaticMesh);
			ISMPointer->ClearInstances();
		}

		UMaterialInstanceDynamic* InstancedMaterial =ISMPointer->CreateAndSetMaterialInstanceDynamic(0);
		

		if(InstancedMaterial != nullptr)
		{
			switch (i)
			{
				case 0:
					InstancedMaterial->SetVectorParameterValue(FName("Color"), AvailableGridColor);
				break;
				
				case 1:
					InstancedMaterial->SetVectorParameterValue(FName("Color"), NotAvailableGridColor);
				break;

				default:
					InstancedMaterial->SetVectorParameterValue(FName("Color"), NotAvailableGridColor);
				break;
			}

			InstancedMaterial->SetScalarParameterValue(FName("Opacity"), 25.f);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "NULL i MAT");
		}

		if(ISMPointer && FloorGridManager)
		{
			//ISMPointer->AddInstance(FTransform{FloorGridManager->GetGridBottomLeft(i)});
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "ADD INSTANCE");		
		}
		
		
	}

	



	/*AGGPlayerController* PlayerController = Cast<AGGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "setset");
		PlayerController->SetGridFloor(this);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "setfail");
	}*/
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

bool AGridFloor::UpdateGridMeshes(TArray<GGGrid*>& GridsToUpdate) const
{

	for(int i = 0; i < GridFloorTypeCount; i++)
	{
		switch (i)
		{
			case 0:
				if(AvailableGridMesh)
				{
					AvailableGridMesh->ClearInstances();
				}
			break;
					
			case 1:
				if(NotAvailableGridMesh)
				{
					NotAvailableGridMesh->ClearInstances();
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
		switch(GridsToUpdate[i]->GridState)
		{
			case EGridState::Empty:
			if(AvailableGridMesh)
			{
				AvailableGridMesh->AddInstanceWorldSpace(FTransform{Pos});
			}
			break;

			case EGridState::Obstacle:
			if(NotAvailableGridMesh)
			{
				NotAvailableGridMesh->AddInstanceWorldSpace(FTransform{Pos});
			}
			break;

			default:
				NotAvailableGridMesh->AddInstanceWorldSpace(FTransform{Pos});
			break;
		}
	}

	return true;
}



