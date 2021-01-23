// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNpcAIController.h"

#include "TownNpcCharacter.h"

void ATownNpcAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    UE_LOG(LogTemp, Warning, TEXT("AI COMPLETE "));
     ATownNpcCharacter* Char = Cast<ATownNpcCharacter>(GetCharacter());
    if(Char)
    {
        if(Char->CurrentBehaviourState == ENpcBehaviourStates::WalkingAround)
        {
            Char->MoveToRandomLocation();
        }
    }
}