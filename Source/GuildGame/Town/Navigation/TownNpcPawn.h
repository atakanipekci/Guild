// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TownNpcPawn.generated.h"

UCLASS()
class GUILDGAME_API ATownNpcPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATownNpcPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveActor(FVector TargetPos);

	UPROPERTY()
	class UFloatingPawnMovement* MovementComponent;
	//class UNavigationSystemV1* NavSystem;

	UPROPERTY()
	class AAIController* AiController;
	
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMesh;

};
