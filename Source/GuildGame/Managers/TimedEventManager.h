// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimedEventManager.generated.h"

enum class ETimedEventType : uint8
{
	Rotation,
	Location
};

USTRUCT()
struct FTargetRotationData
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* OwnerActor;

	FRotator StartRotation;
	FRotator TargetRotation;
	float Duration;
	float Timer;
};

UCLASS()
class GUILDGAME_API ATimedEventManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimedEventManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	
	static ATimedEventManager* ManagerInstance;
	TArray<FTargetRotationData> RotationData;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	static void CreateTimedEvent(UWorld* World);
	static void Rotate(AActor* ActorToRotate, FRotator TargetRotation, float Duration, UWorld* World);
	
};
