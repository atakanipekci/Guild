// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

#include "GGCharacterBase.h"

void UCharacterAnimInstance::ChangeAnimState(ECharacterAnimState State)
{
	if(AnimState == State) return;
	
	AnimState = State;

	// for (int i = 0; i < WeaponAnimInstances.Num(); ++i)
	// {
	// 	if(WeaponAnimInstances[i])
	// 	{
	// 		WeaponAnimInstances[i]->ChangeAnimState(State);
	// 	}
	// }
}

void UCharacterAnimInstance::PlayMontage(UAnimMontage* Montage)
{
	if(Montage)
	{
		Montage_Play(Montage);

		// for (int i = 0; i < WeaponAnimInstances.Num(); ++i)
		// {
		// 	if(WeaponAnimInstances[i])
		// 	{
		// 		WeaponAnimInstances[i]->PlayMontage(Montage);
		// 	}
		// }
	}
}

void UCharacterAnimInstance::SetOwnerCharacter(AGGCharacterBase* NewOwner)
{
	OwnerChar = NewOwner;
}

void UCharacterAnimInstance::OnAttackHitsEnemiesInArea()
{
	if(OwnerChar)
	{
		OwnerChar->OnAttackHitsEnemies();
	}
}

void UCharacterAnimInstance::OnCastingSkillEnds()
{
	if(OwnerChar)
	{
		OwnerChar->OnCastingSkillEnds();
	}
}

void UCharacterAnimInstance::CreateParticleOnTargetGrid()
{
	if(OwnerChar)
	{
		OwnerChar->CreateParticleOnTargetGrid();
	}
}

void UCharacterAnimInstance::ThrowProjectileRightHand()
{
    if(OwnerChar)
	{
		OwnerChar->ThrowProjectile("hand_r_spell", false);
	}
}

void UCharacterAnimInstance::ThrowProjectileLeftHand()
{
    if(OwnerChar)
	{
		OwnerChar->ThrowProjectile("hand_l_spell", false);
	}
}

void UCharacterAnimInstance::ThrowProjectileArrow()
{
    if(OwnerChar)
	{
		OwnerChar->ThrowProjectile("arrow", true);
	}
}
