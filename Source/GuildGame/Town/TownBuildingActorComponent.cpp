// Fill out your copyright notice in the Description page of Project Settings.


#include "TownBuildingActorComponent.h"


#include "TownDefaultPawn.h"
#include "Kismet/GameplayStatics.h"

class ATownDefaultPawn;
// Sets default values for this component's properties
UTownBuildingActorComponent::UTownBuildingActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	
}


// Called when the game starts
void UTownBuildingActorComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BUILDING BEGINPLAY"));
	DayTaskManager::AddBuildingCaller(BuildingDataTableKey, this);
	
	if(PreviewMatBP && ConstructedMatBP && NotConstructedMatBP && UnderConstructionMatBP)
	{
		SetMaterialViaState(ConstructionState);
	}
}
void UTownBuildingActorComponent::SetMaterialViaState(EBuildingConstructionState State)
{
	if(State == EBuildingConstructionState::NotConstructed)
	{
		if(NotConstructedMatInstance == nullptr)
			NotConstructedMatInstance = UMaterialInstanceDynamic::Create(NotConstructedMatBP, this);

		UActorComponent* Building = GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());

		if(Building != nullptr && NotConstructedMatInstance != nullptr)
		{
			Cast<UStaticMeshComponent>(Building)->SetMaterial(0, NotConstructedMatInstance);
		}
	}
	else if(State == EBuildingConstructionState::Constructed)
	{
		if(ConstructedMatInstance == nullptr)
			ConstructedMatInstance = UMaterialInstanceDynamic::Create(ConstructedMatBP, this);

		UActorComponent* Building = GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());

		if(Building != nullptr && ConstructedMatInstance != nullptr)
		{
			Cast<UStaticMeshComponent>(Building)->SetMaterial(0, ConstructedMatInstance);
		}
	}
	else if(State == EBuildingConstructionState::UnderConstruction)
	{
		if(UnderConstructionMatInstance == nullptr)
			UnderConstructionMatInstance = UMaterialInstanceDynamic::Create(UnderConstructionMatBP, this);

		UActorComponent* Building = GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());

		if(Building != nullptr && UnderConstructionMatInstance != nullptr)
		{
			Cast<UStaticMeshComponent>(Building)->SetMaterial(0, UnderConstructionMatInstance);
		}
	}
	else if(State == EBuildingConstructionState::Preview)
	{
		if(PreviewMatInstance == nullptr)
			PreviewMatInstance = UMaterialInstanceDynamic::Create(PreviewMatBP, this);

		UActorComponent* Building = GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());

		if(Building != nullptr && PreviewMatInstance != nullptr)
		{
			Cast<UStaticMeshComponent>(Building)->SetMaterial(0, PreviewMatInstance);
		}
	}
}


// Called every frame
void UTownBuildingActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTownBuildingActorComponent::OnTaskStart()
{
}

void UTownBuildingActorComponent::OnTaskFinish()
{
	 SetConstructionState(EBuildingConstructionState::Constructed);
}

void UTownBuildingActorComponent::OnTaskUpdate()
{
}

EBuildingConstructionState UTownBuildingActorComponent::GetConstructionState() const
{
	return ConstructionState;
}

void UTownBuildingActorComponent::SetConstructionState(EBuildingConstructionState State)
{
	ConstructionState = State;
	SetMaterialViaState(State);
}

