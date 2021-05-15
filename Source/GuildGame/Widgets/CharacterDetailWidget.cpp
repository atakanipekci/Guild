// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDetailWidget.h"

#include "Components/TextBlock.h"
#include "GuildGame/Managers/ImageManager.h"

void UCharacterDetailWidget::RefreshPage(FCharacterStats* Stat)
{
   if(Stat)
   {
       if(CharacterNameText)
       {
           CharacterNameText->SetText(FText::FromString("Char Name"));
       }
       if(CharacterClassText)
       {
           CharacterClassText->SetText(FText::FromString(Stat->ClassName));
       }
       if(HealthText)
       {
           HealthText->SetText(FText::FromString("Health :" + FString::FromInt(Stat->MaxHealth)));
       }
       if(BaseDamageText)
       {
           BaseDamageText->SetText(FText::FromString("Base Damage :" +FString::FromInt(Stat->BaseDamage)));
       }

       if(Portrait)
       {
           ImageManager::SetPortraitTextureByClass(Stat->ClassType, Portrait);
       }
   }
}
