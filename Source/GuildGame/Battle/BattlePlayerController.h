// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePlayerController.generated.h"

enum class EControllerStateIndex: uint8
{
	Placement = 0,
	Movement = 1,
	SkillCast = 2
};

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
	void ChangeStateTo(EControllerStateIndex StateIndex);
	bool UpdateSelectedGrid(bool DrawPathTo = true);
	void DrawPath(int StartIndex, int EndIndex, bool LargeCharacter = false) const;
	void SetGridFloor(class AGridFloor* Grid);
	class AGridFloor* GetGridFloor() const;
	void SelectCharAtMousePos();
	void MoveSelectedChar();
	void SetSelectedCharacter(class AGGCharacter*);
	class AGGCharacter* GetSelectedCharacter() const;
	class AGGCharacter* GetCharacterFromMousePos() const;
	int GetSelectedGridIndex()
	{
		return SelectedGridIndex;
	}
	


private:
	class AGGCharacter* SelectedCharacter = nullptr;
	UPROPERTY()
	class AGridFloor* GridFloor = nullptr;
	int SelectedGridIndex = -1;
	class BattleControllerState* ActiveState;
	int StateIndex;
	TArray<class BattleControllerState*> States;
};
