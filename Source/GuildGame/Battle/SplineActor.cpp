// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"

#include "GGLogHelper.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if(SplineComponent)
	{
		SetRootComponent(SplineComponent);  
	}

	LOG("aspline created");
}

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ClearNodes();
	for(int i = 0; i < NodeCount; i++)
	{
		AddNode(FVector{static_cast<float>(i*200),0,0});
	}
	UpdateSpline();
}

void ASplineActor::AddNode(FVector Position)
{
	if(SplineComponent == nullptr)
	{
		return;
	}

	SplineComponent->AddSplineWorldPoint(Position);
}

void ASplineActor::ClearNodes()
{
	if(SplineComponent == nullptr)
	{
		return;
	}

	SplineComponent->ClearSplinePoints(true);
}

void ASplineActor::UpdateSpline()
{
	if(SplineComponent == nullptr)
	{
		return;
	}
	//ClearNodes();
	//LOG("%d components destroyed", SplineComponent->GetNumChildrenComponents());
	for(int i = SplineComponent->GetNumChildrenComponents()-1; i >= 0; i--)
	{
		USceneComponent* comp = SplineComponent->GetChildComponent(i);
		//comp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//comp->UnregisterComponent();
		if(comp)
		{
			comp->DestroyComponent();
		}
		//SplineComponent->GetChildComponent(i)->DestroyComponent();
	}
	if(SplineComponent && SplineMeshMap.Num() > 0)
	{
		// lookup all pertinent values
		FSplineMeshDetails* StartMeshDetails = nullptr;
		if(SplineMeshMap.Contains(ESplineMeshType::START))
		{
			StartMeshDetails = SplineMeshMap.Find(ESplineMeshType::START);  
		}
		
		FSplineMeshDetails* EndMeshDetails = nullptr;
		if(SplineMeshMap.Contains(ESplineMeshType::END))
		{
			EndMeshDetails = SplineMeshMap.Find(ESplineMeshType::END);
		}

		FSplineMeshDetails* DefaultMeshDetails = nullptr;
		if(SplineMeshMap.Contains(ESplineMeshType::DEFAULT))
		{
			DefaultMeshDetails = SplineMeshMap.Find(ESplineMeshType::DEFAULT);  
		}
		else
		{
			// exit if we don't have a default mesh to work with
			return;
		}

		const int32 SplinePoints = SplineComponent->GetNumberOfSplinePoints();


		//LOG("%d components added", SplinePoints - 1 );
		int ffs = 0;
		for(int SplineCount = 0; SplineCount < (SplinePoints - 1); SplineCount++)
		{

			USplineMeshComponent *SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			UStaticMesh* StaticMesh = DefaultMeshDetails->Mesh;
			UMaterialInterface* Material = nullptr;
			ESplineMeshAxis::Type ForwardAxis = DefaultMeshDetails->ForwardAxis;
			// start mesh
			if(StartMeshDetails && StartMeshDetails->Mesh && SplineCount == 0)
			{
				StaticMesh = StartMeshDetails->Mesh;
				ForwardAxis = StartMeshDetails->ForwardAxis;

				if(StartMeshDetails->DefaultMaterial)
				{
				  Material = StartMeshDetails->DefaultMaterial;
				}          
			}
			else if(EndMeshDetails && EndMeshDetails->Mesh && SplinePoints > 2 && SplineCount == (SplinePoints - 2))
			{
				// end mesh
				StaticMesh = EndMeshDetails->Mesh;
				ForwardAxis = EndMeshDetails->ForwardAxis;

				if(EndMeshDetails->DefaultMaterial)
				{
				  Material = EndMeshDetails->DefaultMaterial;
				}  
			}
			else
			{
				// default assignment - middle mesh
				if(DefaultMeshDetails->AlternativeMaterial && SplineCount > 0 && SplineCount % 2 == 0)
				{
				  Material = DefaultMeshDetails->AlternativeMaterial;
				}
				else if(DefaultMeshDetails->DefaultMaterial)
				{
				  Material = DefaultMeshDetails->DefaultMaterial;
				}
			}

			// update mesh details
			SplineMesh->SetStaticMesh(StaticMesh);
			SplineMesh->SetForwardAxis(ForwardAxis, true);
			SplineMesh->SetMaterial(0, Material);

			SplineComponent->SetSplinePointType(SplineCount,ESplinePointType::Linear,false);

			//SplineMesh->SetRenderCustomDepth(true);
			//SplineMesh->SetCustomDepthStencilValue(5);


			// initialize the object
			SplineMesh->RegisterComponent();
			//SplineMesh->RegisterComponentWithWorld(GetWorld());

			SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			SplineMesh->SetMobility(EComponentMobility::Movable);

			bool ffsbool = SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
			if(ffsbool)
			{
				ffs++;
			}
			
			//LOG("%d total child components", SplineComponent->GetNumChildrenComponents());
			// define the positions of the points and tangents
			const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Type::Local);
			const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Type::Local);
			const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Type::Local);
			const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Type::Local);
			SplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

			// query physics
			SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		//LOG("%d reee components added", ffs );
		
	}
	SplineComponent->UpdateSpline();
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

