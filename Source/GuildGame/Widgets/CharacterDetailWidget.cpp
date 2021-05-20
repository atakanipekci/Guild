// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDetailWidget.h"

#include "CharacterSkillsWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GuildGame/Managers/ImageManager.h"
#include "GuildGame/Managers/WidgetManager.h"


void UCharacterDetailWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    if(SkillsButton)
    {
        SkillsButton->OnClicked.AddUniqueDynamic(this, &UCharacterDetailWidget::OpenSkillsWidget);
    }
}

void UCharacterDetailWidget::RefreshPage(FCharacterStats* Stat)
{
   if(Stat)
   {
       CharacterStat = Stat;
       
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

void UCharacterDetailWidget::OpenSkillsWidget()
{
    if(CharacterStat == nullptr) return;;
    
     UUserWidget* SkillsPage = WidgetManager::GetSkillsWidgetByType(CharacterStat->ClassType, this);
     if(SkillsPage)
     {
         SkillsPage->SetVisibility(ESlateVisibility::Visible);
         if(SkillsPage->IsInViewport() == false)
         {
             SkillsPage->AddToViewport();
         }
     }
     UCharacterSkillsWidget* SkillsWidget =  Cast<UCharacterSkillsWidget>(SkillsPage);
     if(SkillsWidget)
     {
         SkillsWidget->RefreshPage(CharacterStat);
     }
}
