// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDetailWidget.h"

#include "CharacterSkillsWidget.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"
#include "GuildGame/Managers/ImageManager.h"
#include "GuildGame/Managers/WidgetManager.h"


void UCharacterDetailWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // if(SkillsButton)
    // {
    //     SkillsButton->SetVisibility(ESlateVisibility::Hidden);
    //    // SkillsButton->OnClicked.AddUniqueDynamic(this, &UCharacterDetailWidget::OpenSkillsWidget);
    // }
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
           BaseDamageText->SetText(FText::FromString("Base Damage :" + FString::FromInt(Stat->BaseDamage)));
       }

       // if(SkillsButtonText)
       // {
       //     SkillsButtonText->SetText(FText::FromString("Skills:" + FString::FromInt(Stat->SpendableSkillPoints)));
       // }
       

       if(Portrait)
       {
           ImageManager::SetPortraitTextureByClass(Stat->ClassType, Portrait);
       }

       OpenSkillsWidget();
   }
}

void UCharacterDetailWidget::OpenSkillsWidget()
{
    if(CharacterStat == nullptr) return;
    
     UUserWidget* SkillsPage = AWidgetManager::GetSkillsWidgetByType(CharacterStat->ClassType, GetWorld());
     if(SkillsPage)
     {
         SkillsPage->SetVisibility(ESlateVisibility::Visible);
         // if(SkillsPage->IsInViewport() == false)
         // {
         //     SkillsPage->AddToViewport();
         // }
         if(SkillsPanelParent)
         {
            SkillsPanelParent->ClearChildren();
   	   	    SkillsPanelParent->AddChild(SkillsPage);
         }
     }
     UCharacterSkillsWidget* SkillsWidget =  Cast<UCharacterSkillsWidget>(SkillsPage);
     if(SkillsWidget)
     {
         SkillsWidget->RefreshPage(CharacterStat);
     }
}


