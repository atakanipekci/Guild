// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GuildGame/Managers/CharacterManager.h"
#include "CoreMinimal.h"
#include "GuildGame/GridSystem/Grid.h"
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
	TArray<Grid*>* GetMovableGrids();
	TArray<Grid*>* GetDamageableGrids();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetStats(const FCharacterStats&);
	void MoveTo(FVector);
	void UpdateMovableGrids();
	void UpdateDamageableGrids();
	void AttackTo(AGGCharacter* Target);
	void SetSelected();
	void Deselect();
	void SetCurrentIndex(int NewIndex);
	float GetDefaultMovementRange()const;
	float GetDefaultDamageRange()const;
	int GetBaseDamage()const;
	ECharacterStatus GetStatus()const;
	void SetStatus(ECharacterStatus);
	float TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator, AActor * DamageCause) override;
	float TakeDefaultDamage(float DamageAmount, AActor* Dealer);
	void ShowDamageableGrids();

private:
	TArray<Grid*> MovableGrids;
	TArray<Grid*> DamageableGrids;
	ECharacterStatus Status = ECharacterStatus::Idle;
	class UCharacterStatsComponent* StatsComponent;
	int CurrentGridIndex;
};
