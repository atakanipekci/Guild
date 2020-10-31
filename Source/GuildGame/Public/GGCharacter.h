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
	TArray<GGGrid*>* GetMovableGrids();
	TArray<GGGrid*>* GetDamageableGrids();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveTo(FVector);
	void UpdateMovableGrids();
	void UpdateDamageableGrids();
	void AttackTo(AGGCharacter* Target);
	void SetSelected();
	float GetDefaultMovementRange()const;
	float GetDefaultDamageRange()const;
	int GetBaseDamage()const;
	ECharacterStatus GetStatus()const;
	void SetStatus(ECharacterStatus);
	float TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator, AActor * DamageCause) override;
	float TakeDefaultDamage(float DamageAmount, AActor* Dealer);
	void ShowDamageableGrids();

private:
	TArray<GGGrid*> MovableGrids;
	TArray<GGGrid*> DamageableGrids;
	ECharacterStatus Status = ECharacterStatus::Idle;
	class UCharacterStatsComponent* StatsComponent;
};
