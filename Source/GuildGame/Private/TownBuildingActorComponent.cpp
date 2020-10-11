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

	if(TransparentMatBlueprint && DefaultMatBlueprint)
	{
		SetMaterialViaState(IsBuildingConstructed);
	}
}
void UTownBuildingActorComponent::SetMaterialViaState(bool IsConstructed)
{
	if(IsConstructed == false)
	{
		if(TransparentMatInstance == nullptr)
			TransparentMatInstance = UMaterialInstanceDynamic::Create(TransparentMatBlueprint, this);

		UActorComponent* Building = GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());

		if(Building != nullptr && TransparentMatInstance != nullptr)
		{
			Cast<UStaticMeshComponent>(Building)->SetMaterial(0, TransparentMatInstance);
		}
	}
	else
	{
		if(DefaultMatInstance == nullptr)
			DefaultMatInstance = UMaterialInstanceDynamic::Create(DefaultMatBlueprint, this);

		UActorComponent* Building = GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());

		if(Building != nullptr && DefaultMatInstance != nullptr)
		{
			Cast<UStaticMeshComponent>(Building)->SetMaterial(0, DefaultMatInstance);
		}
	}
}


// Called every frame
void UTownBuildingActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ULevelSequence* UTownBuildingActorComponent::GetSequenceAsset() const
{
	ATownDefaultPawn* DefaultPawn = Cast<ATownDefaultPawn>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
	//UMyGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if(DefaultPawn)
	{
		return DefaultPawn->GetMappedSequenceAsset(BuildingDataKey);
	}
	
	return  nullptr;
}

