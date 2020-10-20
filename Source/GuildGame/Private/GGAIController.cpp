// Fill out your copyright notice in the Description page of Project Settings.


#include "GGAIController.h"

#include "GGCharacter.h"

void AGGAIController::OnMoveCompleted(FAIRequestID Id, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(Id, Result);
    AGGCharacter* Char = Cast<AGGCharacter>(GetCharacter());
    if(Char)
    {
        if(Char->GetStatus() == ECharacterStatus::Moving)
        {
            Char->SetStatus(ECharacterStatus::Idle);
        }
    }
}
