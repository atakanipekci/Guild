// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNpcAIController.h"

#include "TownNpcCharacter.h"
#include "GuildGame/Characters/CharacterAnimInstance.h"

void ATownNpcAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
     ATownNpcCharacter* Char = Cast<ATownNpcCharacter>(GetCharacter());
    if(Char)
    {
        if(Char->CurrentBehaviourState == ENpcBehaviourStates::WalkingAround)
        {
            Char->SetAnimState(ECharacterAnimState::Idle);
            //Char->MoveToRandomLocation();
        }
    }
}
