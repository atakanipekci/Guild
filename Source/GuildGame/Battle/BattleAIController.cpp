// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAIController.h"
#include "GGLogHelper.h"
#include "GuildGame/Characters/CharacterAnimInstance.h"
#include "GuildGame/Characters/GGCharacter.h"

void ABattleAIController::OnMoveCompleted(FAIRequestID Id, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(Id, Result);
    AGGCharacter* Char = Cast<AGGCharacter>(GetCharacter());
    if(Char)
    {
        if(Char->GetStatus() == ECharacterStatus::Moving)
        {
            Char->SetStatus(ECharacterStatus::Idle);

            UCharacterAnimInstance* AnimInstance = Char->GetAnimInstance();
            if(AnimInstance)
			    AnimInstance->ChangeAnimState(ECharacterAnimState::Idle);
        }
    }
}
