// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAIController.h"
#include "GGLogHelper.h"
#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Characters/CharacterAnimInstance.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/Widgets/BattleHudWidget.h"
#include "Kismet/GameplayStatics.h"

void ABattleAIController::OnMoveCompleted(FAIRequestID Id, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(Id, Result);
    AGGCharacter* Char = Cast<AGGCharacter>(GetCharacter());
    if(Char)
    {
        if(Char->GetStatus() == ECharacterStatus::Moving)
        {
            Char->SetStatus(ECharacterStatus::Idle);
            Char->SetAnimState(ECharacterAnimState::Idle);
        }
    }

    AGuildGameGameModeBase* BattleGameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(BattleGameMode)
	{
		if(BattleGameMode->HudWidgetInstance)
		{
			BattleGameMode->HudWidgetInstance->SetSkillsPanelVisible();
		}
	}
}
