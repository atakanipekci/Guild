// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TownNavNodeActor.generated.h"

enum class ENpcBehaviourStates: uint8;

UCLASS()
class GUILDGAME_API ATownNavNodeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATownNavNodeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Pathfinding)
	TArray<ATownNavNodeActor*> AvailableNodes;

	ENpcBehaviourStates NodeType;
	FVector Destination;
};
