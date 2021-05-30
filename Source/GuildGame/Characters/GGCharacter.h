// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GuildGame/Managers/CharacterManager.h"
#include "CoreMinimal.h"

#include "GGCharacterBase.h"
#include "GuildGame/GridSystem/Grid.h"
#include "GameFramework/Character.h"
//#include "GuildGame/Skills/CharacterSkills.h"

#include "GGCharacter.generated.h"

enum class ECharacterStatus : uint8
{
	Idle = 0,
	Moving = 1
};

UCLASS()
class GUILDGAME_API AGGCharacter : public AGGCharacterBase
{
	GENERATED_BODY()

	friend class CharacterManager;
public:
	// Sets default values for this character's properties
	AGGCharacter();
	~AGGCharacter();
	TArray<Grid*>* GetMovableGrids();
	TArray<Grid*>* GetTargetableGrids();
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
	void UpdateTargetableGrids(const struct FSkillData*);
	void UpdateDamageableGrids( const class CharacterSkill* Skill, int CenterIndex);
	void AttackTo(AGGCharacter* Target);
	void SetSelected();
	void Deselect();
	void SetCurrentIndex(int NewIndex);
	int GetCurrentIndex() const
	{
		return CurrentGridIndex;
	}

	int GetCurrentTargetIndex() const
	{
		return CurrentTargetGridIndex;
	}
	float GetDefaultMovementRange()const;
	float GetDefaultDamageRange()const;
	int GetBaseDamage()const;
	ECharacterStatus GetStatus()const;
	void SetStatus(ECharacterStatus);
	float TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator, AActor * DamageCause) override;
	float TakeDefaultDamage(float DamageAmount, AActor* Dealer);
	float Heal(float HealAmount, AGGCharacter* Healer);
	void ShowTargetableGrids();
	void ShowDamageableGrids(int CenterIndex, bool CreateNew = true);

	void CastSkill(TArray<AGGCharacter*>& TargetCharacters);

	void UpdateHealthBar();
	class UCharacterAnimInstance* GetAnimInstance();


private:
	TArray<Grid*> MovableGrids;
	TArray<Grid*> TargetableGrids;
	TArray<Grid*> DamageableGrids;
	ECharacterStatus Status = ECharacterStatus::Idle;

	UPROPERTY()
	class UCharacterStatsComponent* StatsComponent;
	
	UPROPERTY()
	class UCharacterAnimInstance* AnimInstance;
	
	
	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* HealthBarComponent;
	UPROPERTY()
	class UBattleHealthBarWidget* HealthBarWidget;
	
	int CurrentGridIndex;
	TArray<class CharacterSkill*> Skills;
	int CurrentSkillIndex = 0;

	int CurrentTargetGridIndex = 0;
};
