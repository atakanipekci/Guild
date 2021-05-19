// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

void UCharacterAnimInstance::ChangeAnimState(ECharacterAnimState State)
{
	if(AnimState == State) return;
	
	AnimState = State;
}
