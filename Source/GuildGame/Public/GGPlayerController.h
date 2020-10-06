// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GridFloor.h"
#include "CoreMinimal.h"
#include "GGCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API AGGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AGGPlayerController();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void UpdateSelectedGrid();
	void SetGridFloor(AGridFloor* Grid);
	void SelectCharAtMousePos();
	void MoveSelectedChar();
	void SetSelectedCharacter(AGGCharacter*);


private:
	class AGGCharacter* SelectedCharacter = nullptr;
	AGridFloor* GridFloor = nullptr;
	int SelectedGridIndex = -1;
};
