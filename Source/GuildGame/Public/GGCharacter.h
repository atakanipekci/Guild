// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterManager.h"
#include "CoreMinimal.h"
#include "GGGrid.h"
#include "GameFramework/Character.h"
#include "GGCharacter.generated.h"

enum class ECharacterStatus : uint8
{
	Idle = 0,
	Moving = 1
};

UCLASS()
class GUILDGAME_API AGGCharacter : public ACharacter
{
	GENERATED_BODY()

	friend class CharacterManager;
public:
	// Sets default values for this character's properties
	AGGCharacter();
	void SetActive();
	TArray<GGGrid*>* GetMovableGrids();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveTo(FVector);
	void UpdateMovableGrids();
	void SetSelected();
	float GetDefaultMovementRange()const;
	ECharacterStatus GetStatus()const;
	void SetStatus(ECharacterStatus);

private:
	float MovementRange = 850;
	TArray<GGGrid*> MovableGrids;
	ECharacterStatus Status = ECharacterStatus::Idle;
};
