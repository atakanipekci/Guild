// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterManager.h"
#include "CoreMinimal.h"
#include "GGGrid.h"
#include "GameFramework/Character.h"
#include "GGCharacter.generated.h"

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

	float MovementRange = 315;
private:
	TArray<GGGrid*> MovableGrids;
};
