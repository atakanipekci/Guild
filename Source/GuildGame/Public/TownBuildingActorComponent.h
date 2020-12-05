// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WidgetManager.h"
#include "Components/ActorComponent.h"
#include "UObject/ObjectMacros.h"


#include "TownBuildingActorComponent.generated.h"

UENUM(BlueprintType)
enum class EBuildingConstructionState: uint8
{
	NotConstructed,
	UnderConstruction,
	Constructed,
	Preview
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUILDGAME_API UTownBuildingActorComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UTownBuildingActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetMaterialViaState(EBuildingConstructionState State);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EBuildingConstructionState GetConstructionState() const;
	void SetConstructionState(EBuildingConstructionState State);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	FString BuildingDataTableKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	ESequenceKeys SequenceKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	EWidgetKeys WidgetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	UMaterialInstance* PreviewMatBP;
	UPROPERTY(BlueprintReadWrite, Category = Building)
	UMaterialInstanceDynamic* PreviewMatInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	UMaterialInstance* ConstructedMatBP;
	UPROPERTY(BlueprintReadWrite, Category = Building)
	UMaterialInstanceDynamic* ConstructedMatInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	UMaterialInstance* NotConstructedMatBP;
	UPROPERTY(BlueprintReadWrite, Category = Building)
	UMaterialInstanceDynamic* NotConstructedMatInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	UMaterialInstance* UnderConstructionMatBP;
	UPROPERTY(BlueprintReadWrite, Category = Building)
	UMaterialInstanceDynamic* UnderConstructionMatInstance;


private:

	UPROPERTY(EditAnywhere, Category = Building)
	EBuildingConstructionState ConstructionState;
		
};
