// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABattlePlayerController();
	~ABattlePlayerController();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	class BattleControllerState* GetActiveState() const;
	void SetState(int StateIndex);
	void ChangeStateTo(int StateIndex);
	void UpdateSelectedGrid(bool DrawPathTo = true);
	void DrawPath(int StartIndex, int EndIndex) const;
	void SetGridFloor(class AGridFloor* Grid);
	class AGridFloor* GetGridFloor() const;
	void SelectCharAtMousePos();
	void MoveSelectedChar();
	void SetSelectedCharacter(class AGGCharacter*);
	class AGGCharacter* GetSelectedCharacter() const;
	class AGGCharacter* GetCharacterFromMousePos() const;
	


private:
	class AGGCharacter* SelectedCharacter = nullptr;
	class AGridFloor* GridFloor = nullptr;
	int SelectedGridIndex = -1;
	class BattleControllerState* ActiveState;
	TArray<class BattleControllerState*> States;
};