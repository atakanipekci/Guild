// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */

UENUM()
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
	UFUNCTION(BlueprintCallable)
	void ChangeAnimState(ECharacterAnimState State);

	void PlayMontage(UAnimMontage* Montage);

	UPROPERTY()
	class AGGCharacterBase* OwnerChar;
	
	void SetOwnerCharacter(class AGGCharacterBase*);

	UFUNCTION(BlueprintCallable)
	void OnAttackHitsEnemy();
	UFUNCTION(BlueprintCallable)
	void OnCastingSkillEnds();
	
};
