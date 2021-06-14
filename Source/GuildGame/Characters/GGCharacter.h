// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterStats.h"
#include "GGCharacterBase.h"
#include "GuildGame/GridSystem/Grid.h"
#include "GuildGame/Managers/StatusEffectManager.h"
//#include "GuildGame/Skills/CharacterSkills.h"

#include "GGCharacter.generated.h"

DECLARE_DYNAMIC_DELEGATE(FSkillCastedDelegate);

enum class ECharacterAnimState : uint8;

enum class ECharacterStatus : uint8
{
	Idle = 0,
	Moving = 1,
	Dead = 2,
	Casting = 3
};

struct FCooldownTimer
{
private:
	int Timer = 0;
	int Cooldown = 0;

public:

	FSkillCastedDelegate OnSkillCastedDelegate;

	FCooldownTimer(float Cooldown)
	{
		this->Cooldown = Cooldown;
	}
	
	void SetCooldown(float NewCooldown)
	{
		Cooldown = NewCooldown;
		Timer = 0;
	}

	void RestartTimer()
	{
		Timer = Cooldown;
	}

	void DecreaseTurn(int Amount) 
	{
		if(Timer > 0)
		{
			Timer -= Amount;
			Timer = FMath::Max(Timer, 0);
		}
	}
	
	bool IsTimeUp() const
	{
		if(Timer <= 0)
		{
			return  true;
		}
		return  false;
	}

	float HowMuchLeft() const
	{
		return Timer;
	}
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
	int GetSpeed() const;
	int GetBaseDamage()const;
	ECharacterStatus GetStatus()const;
	void SetStatus(ECharacterStatus);
	float TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator, AActor * DamageCause) override;
	float TakeDefaultDamage(float DamageAmount, AActor* Dealer);
	float Heal(float HealAmount, AGGCharacter* Healer);
	void ShowTargetableGrids();
	void ShowDamageableGrids(int CenterIndex, bool CreateNew = true);
	void UpdateCurrentGridIndex();

	void SetCustomDepth(bool Active, int StencilValue);

	void CastSkill(TArray<AGGCharacter*>& TargetCharacters);
	virtual void OnAttackHitsEnemies() override;
	virtual void OnAttackHitsEnemy(AActor* TargetToHit) override;
	virtual void OnDeath() override;
	UFUNCTION()
	virtual void OnCastingSkillEnds() override;

	
	AActor* CreateProjectile(FName SocketName, bool bUseBoneRotation);
	virtual void CreateParticleOnTargetGrid() override;
	virtual void CreateParticlesOnEveryEnemyInDamageable() override;

	
	virtual void ThrowProjectile(FName SocketName, bool bUseBoneRotation) override;

	void UpdateHealthBar();
	class UCharacterAnimInstance* GetAnimInstance();
	void PrepareAnimInstance();

	void SetAnimState(ECharacterAnimState AnimState);
	void PlayCharacterMontage(UAnimMontage* Montage);

	class CharacterSkill* GetCurrentSkill();
	TArray<class CharacterSkill*>* GetSkills();

	bool SetCurrentSkillIfContains(int SkillID);
	
	FVector GetTargetTrajectoryLocation();
	FVector GetStartTrajectoryLocation();

	AGGCharacter* GetCharacterAtTargetGridIndex();
	bool CanTrajectoryBeShown();

	TMap<int, FCooldownTimer> SkillsCooldownMap;

	void OnTurnEnds();
	void OnIndividualTurnBegins();
	void OnIndividualTurnEnds();
	
	TMap<EStatusEffectType,  FStatusEffectData>* GetAppliedStatusEffects();

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
	
	TArray<class CharacterSkill*> Skills;
	int CurrentGridIndex;
	int CurrentSkillIndex = 0;
	int CurrentTargetGridIndex = 0;

	TArray<AGGCharacter*> SelectedTargetCharacters;

	TMap<EStatusEffectType, struct FStatusEffectData> AppliedStatusEffects;


	bool bIsSkillMontagePlaying;

	struct FCharFileDataTable CharFile;
};
