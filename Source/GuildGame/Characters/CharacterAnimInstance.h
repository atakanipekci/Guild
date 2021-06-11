// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterStats.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */

UENUM(Blueprintable)
enum class ECharacterAnimState: uint8
{
	Idle,
	Run
};

UCLASS()
class GUILDGAME_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterAnimState AnimState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterClassType ClassType;
	
	UFUNCTION(BlueprintCallable)
	void ChangeAnimState(ECharacterAnimState State);

	void PlayMontage(UAnimMontage* Montage);

	UPROPERTY()
	class AGGCharacterBase* OwnerChar;

	UPROPERTY()
	TArray<UCharacterAnimInstance*>  WeaponAnimInstances;
	
	void SetOwnerCharacter(class AGGCharacterBase*);

	UFUNCTION(BlueprintCallable)
	void OnAttackHitsEnemiesInArea();
	UFUNCTION(BlueprintCallable)
	void OnCastingSkillEnds();

	UFUNCTION(BlueprintCallable)
	void CreateParticleOnTargetGrid();
	
	UFUNCTION(BlueprintCallable)
	void ThrowProjectileRightHand();
	
	UFUNCTION(BlueprintCallable)
	void ThrowProjectileLeftHand();

	UFUNCTION(BlueprintCallable)
	void ThrowProjectileArrow();

	
};
