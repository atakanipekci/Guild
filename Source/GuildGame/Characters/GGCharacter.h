// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterStats.h"
#include "GGCharacterBase.h"
#include "GuildGame/GridSystem/Grid.h"
//#include "GuildGame/Skills/CharacterSkills.h"

#include "GGCharacter.generated.h"

enum class ECharacterAnimState : uint8;

enum class ECharacterStatus : uint8
{
	Idle = 0,
	Moving = 1,
	Dead = 2,
	Casting = 3
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
	void SetStats(const struct FCharacterStats&);
	void SetFile(const struct FCharFileDataTable&);
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
	virtual void OnAttackHitsEnemies() override;
	virtual void OnAttackHitsEnemy(AActor* TargetToHit) override;
	virtual void OnDeath() override;
	UFUNCTION()
	virtual void OnCastingSkillEnds() override;

	
	AActor* CreateProjectile(FName SocketName);
	virtual void ThrowProjectileRightHand() override;
	virtual void ThrowProjectileLeftHand() override;

	void UpdateHealthBar();
	class UCharacterAnimInstance* GetAnimInstance();
	void PrepareAnimInstance();

	void SetAnimState(ECharacterAnimState AnimState);
	void PlayCharacterMontage(UAnimMontage* Montage);

	class CharacterSkill* GetCurrentSkill();

	FVector GetTargetTrajectoryLocation();
	FVector GetStartTrajectoryLocation();

	AGGCharacter* GetCharacterAtTargetGridIndex();

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

	UPROPERTY()
	class USplineComponent* SplineComponent;
	
	int CurrentGridIndex;
	TArray<class CharacterSkill*> Skills;
	int CurrentSkillIndex = 0;

	int CurrentTargetGridIndex = 0;

	TArray<AGGCharacter*> SelectedTargetCharacters;

	

	struct FCharFileDataTable CharFile;
};
