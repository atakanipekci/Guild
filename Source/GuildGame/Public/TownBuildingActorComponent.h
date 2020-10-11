// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TownBuildingActorComponent.generated.h"


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
	void SetMaterialViaState(bool IsConstructed);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
    FString BuildingDataKey;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	UMaterialInstance* TransparentMatBlueprint;
	UPROPERTY(BlueprintReadWrite, Category = Building)
	UMaterialInstanceDynamic* TransparentMatInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	
	UMaterialInstance* DefaultMatBlueprint;
	UPROPERTY(BlueprintReadWrite, Category = Building)
	UMaterialInstanceDynamic* DefaultMatInstance;

	class ULevelSequence* GetSequenceAsset() const;

	bool IsConstructed() const;

private:
	UPROPERTY(EditAnywhere, Category = Building)
	bool IsBuildingConstructed = true;
		
};
