// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

#include "GGCharacterBase.h"

void UCharacterAnimInstance::ChangeAnimState(ECharacterAnimState State)
{
	if(AnimState == State) return;
	
	AnimState = State;
}

void UCharacterAnimInstance::PlayMontage(UAnimMontage* Montage)
{
	if(Montage)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayMontage"));
		Montage_Play(Montage);
	}
}

void UCharacterAnimInstance::SetOwnerCharacter(AGGCharacterBase* NewOwner)
{
	OwnerChar = NewOwner;
}

void UCharacterAnimInstance::OnAttackHitsEnemy()
{
	if(OwnerChar)
	{
		OwnerChar->OnAttackHitsEnemy();
	}
}
