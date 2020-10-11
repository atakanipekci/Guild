// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TownDefaultPawn.generated.h"

USTRUCT(BlueprintType)
	 struct FBuildingData
	 {
	     GENERATED_USTRUCT_BODY()
	 
	     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	     class ULevelSequence* SequenceAsset;

 		 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	     class UTownBuildingWidgetBase* UiWidget;

		 UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	     UTownBuildingWidgetBase* UiWidgetInstance;
	 };

UCLASS()
class GUILDGAME_API ATownDefaultPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATownDefaultPawn();

	ATownDefaultPawn(const FObjectInitializer& ObjectInitializer);

	void LeftClickHandler();
	void RightClickHandler();
	void PlaySequence(TWeakObjectPtr<AActor> Actor);
	void PlaySequenceReverse();

	UFUNCTION(BlueprintCallable)
	void CreateWidgetViaCode();
	UFUNCTION()
	void CollapseBuildingWidgetOnAnimationFinish();

	UFUNCTION()
	void EnablePlayerInputOnSequenceFinish();
	
	UFUNCTION(BlueprintCallable)
	void SetMappedWidgetInstance(FString Key, UTownBuildingWidgetBase* Widget);
	
	UFUNCTION(BlueprintCallable)
    UTownBuildingWidgetBase* GetMappedWidget(FString Key);
	
	UFUNCTION(BlueprintCallable)
    UTownBuildingWidgetBase* GetMappedWidgetInstance(FString Key);
	
	UFUNCTION(BlueprintCallable)
	ULevelSequence* GetMappedSequenceAsset(FString Key);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, Category = Camera)
    bool IsReversed;

	UPROPERTY(BlueprintReadWrite, Category = Camera)
	class UTownBuildingActorComponent* SelectedBuilding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    TMap<FString, FBuildingData > BuildingDataMap;


private:
	UPROPERTY()
    ULevelSequence* SequenceAsset;

	UPROPERTY()
	class ALevelSequenceActor* SequenceActor;

	UPROPERTY()
    class ULevelSequencePlayer* SequencePlayer;

	UStaticMeshComponent* HoveredBuildingStaticMeshComp;

	bool bIsBuildingFocused = false;

	bool bEnableInput = true;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
