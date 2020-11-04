// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "TownScrollBoxItem.h"
#include "GameFramework/Pawn.h"
#include "TownDefaultPawn.generated.h"

enum class EBuildingTypes : unsigned char;

USTRUCT(BlueprintType)
	 struct FBuildingData
	 {
	     GENERATED_USTRUCT_BODY()
	 
	     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	     class ULevelSequence* SequenceAsset;

 		 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	     class UTownBuildingWidgetBase* UiWidget;

		 UPROPERTY(BlueprintReadWrite, Category = Camera)
	     UTownBuildingWidgetBase* UiWidgetInstance;
	 };

USTRUCT(BlueprintType)
	 struct FScrollItem
	 {
	     GENERATED_USTRUCT_BODY()

		 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		 class UTownScrollBoxItem* ScrollItem;
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
	void ZoomOutFromBuilding();
	void PlaySequence(TWeakObjectPtr<AActor> Actor);
	void PlaySequenceReverse();

	UFUNCTION(BlueprintCallable)
	void CreateWidgetViaCode();
	UFUNCTION()
	void CollapseBuildingWidgetOnAnimationFinish();

	UFUNCTION()
	void EnablePlayerInputOnSequenceFinish();
	
	UFUNCTION(BlueprintCallable)
	void SetMappedWidgetInstance(const EBuildingTypes Key, UTownBuildingWidgetBase* Widget);
	
	UFUNCTION(BlueprintCallable)
    UTownBuildingWidgetBase* GetMappedWidget(const EBuildingTypes Key);
	
	UFUNCTION(BlueprintCallable)
    UTownBuildingWidgetBase* GetMappedWidgetInstance(const EBuildingTypes Key);
	
	UFUNCTION(BlueprintCallable)
	ULevelSequence* GetMappedSequenceAsset(const EBuildingTypes Key);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, Category = Camera)
    bool IsReversed;

	UPROPERTY(BlueprintReadWrite, Category = Camera)
	class UTownBuildingActorComponent* SelectedBuilding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    TMap<EBuildingTypes, FBuildingData > BuildingDataMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
    TMap<EScrollBoxType, FScrollItem > ScrollItemsMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UTownYesOrNoWidget* YesOrNoWidgetBP;

	


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
