// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharSkillNodeWidget.h"

#include "BattleHudWidget.h"
#include "CharacterSkillTooltipWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GuildGame/Battle/BattlePlayerController.h"
#include "Kismet/GameplayStatics.h"

void UBattleCharSkillNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(SkillButton && PortraitBg)
	{
		SkillButton->OnClicked.AddUniqueDynamic(this, &UBattleCharSkillNodeWidget::OnPressed);
		ButtonStyle = SkillButton->WidgetStyle;

		NormalBrush  = PortraitBg->Brush;
	}
}

void UBattleCharSkillNodeWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(Tooltip)
	{
		Tooltip->Refresh(SkillsData, SkillTexture, &DescText);
	}
}

void UBattleCharSkillNodeWidget::OnPressed()
{
	if(bIsMovementButton == false)
	{
		if(SelectedCharacter && SkillCooldownData)
		{
			if(SkillCooldownData->IsTimeUp() == false) return;

			SelectedCharacter->SetCurrentSkillIfContains(SkillsData.SkillID);

			if(PlayerController) PlayerController->ChangeStateTo(EControllerStateIndex::SkillCast);

			if(OwnerHud)
			{
				OwnerHud->RefreshSkillButtonsState();
			}
		}
	}
	else
	{
		if(PlayerController) PlayerController->ChangeStateTo(EControllerStateIndex::Movement);

		if(SelectedCharacter)
			SelectedCharacter->ShowMovableGrids(true);

		if(OwnerHud)
		{
			OwnerHud->RefreshSkillButtonsState();
		}
	}
}

void UBattleCharSkillNodeWidget::RefreshNodeState()
{
	if(bIsMovementButton == false)
	{
		if(SkillButton && SkillButton->GetIsEnabled() == false)
		{
			SkillButton->SetIsEnabled(true);
		}
		
		if(SkillCooldownData && PlayerController && SelectedCharacter)
		{
			if(SkillCooldownData->IsTimeUp() == false)
			{
				CooldownPanel->SetVisibility(ESlateVisibility::Visible);
				if(PortraitBg)
				{
					PortraitBg->SetBrushResourceObject(CooldownBrush.GetResourceObject());
					PortraitBg->SetBrushSize(NormalBrush.GetImageSize());
					PortraitBg->SetBrushTintColor(CooldownBrush.TintColor);
				}
				CooldownPanel->SetVisibility(ESlateVisibility::Visible);
				if(CooldownText)
				{
					const float CooldownTimer = SkillCooldownData->HowMuchLeft();
					CooldownText->SetText(FText::AsNumber(CooldownTimer));
				}
			}
			else
			{
				CooldownPanel->SetVisibility(ESlateVisibility::Hidden);
				if(PlayerController->GetStateType() == EControllerStateIndex::Movement)
				{
					if(PortraitBg)
					{
						PortraitBg->SetBrushResourceObject(NormalBrush.GetResourceObject());
						PortraitBg->SetBrushTintColor(NormalBrush.TintColor);
					}
				}
				else if(SelectedCharacter->GetCurrentSkill() && SelectedCharacter->GetCurrentSkill()->GetSkillID() == SkillsData.SkillID)
				{
					if(PortraitBg)
					{
						PortraitBg->SetBrushResourceObject(PressedBrush.GetResourceObject());
						PortraitBg->SetBrushSize(NormalBrush.GetImageSize());
						PortraitBg->SetBrushTintColor(PressedBrush.TintColor);
					}
				}
				else
				{
					if(PortraitBg)
					{
						PortraitBg->SetBrushResourceObject(NormalBrush.GetResourceObject());
						PortraitBg->SetBrushTintColor(NormalBrush.TintColor);
					}
				}

				int ApCost = 0;
				CharacterSkill * Skill = SelectedCharacter->GetOwnedSkillbyID(SkillsData.SkillID);
				
				if(SkillButton && SelectedCharacter && SelectedCharacter->IsApEnoughForSkill(Skill, ApCost) == false)
				{
					SkillButton->SetIsEnabled(false);
					if(PortraitBg)
					{
						PortraitBg->SetBrushResourceObject(NormalBrush.GetResourceObject());
						PortraitBg->SetBrushTintColor(NormalBrush.TintColor);
					}
				}
			}
		}
	}
	else
	{
		if(SkillButton && SkillButton->GetIsEnabled() == false)
		{
			SkillButton->SetIsEnabled(true);
		}

		if(SelectedCharacter && SelectedCharacter->GetCurrentAP() <= 0)
		{
			SkillButton->SetIsEnabled(false);
			if(PortraitBg)
			{
				PortraitBg->SetBrushResourceObject(NormalBrush.GetResourceObject());
				PortraitBg->SetBrushTintColor(NormalBrush.TintColor);
			}
			
		}
		else
		{
			if(PlayerController->GetStateType() != EControllerStateIndex::Movement)
			{
				if(PortraitBg)
				{
					PortraitBg->SetBrushResourceObject(NormalBrush.GetResourceObject());
					PortraitBg->SetBrushTintColor(NormalBrush.TintColor);
				}
			}
			else
			{
				if(PortraitBg)
				{
					PortraitBg->SetBrushResourceObject(PressedBrush.GetResourceObject());
					PortraitBg->SetBrushSize(NormalBrush.GetImageSize());
					PortraitBg->SetBrushTintColor(PressedBrush.TintColor);
				}
			}
		}
	}
}

void UBattleCharSkillNodeWidget::RefreshNode(AGGCharacter* SelectedChar, UBattleHudWidget* Hud)
{
	if(SelectedChar == nullptr) return;

	if(SkillButton && SkillTexture)
	{
		SkillButton->WidgetStyle.Normal.SetResourceObject(SkillTexture);
		SkillButton->WidgetStyle.Normal.SetImageSize(ButtonStyle.Normal.ImageSize);
		SkillButton->WidgetStyle.Hovered.SetResourceObject(SkillTexture);
		SkillButton->WidgetStyle.Hovered.SetImageSize(ButtonStyle.Hovered.ImageSize);
		SkillButton->WidgetStyle.Pressed.SetResourceObject(SkillTexture);
		SkillButton->WidgetStyle.Pressed.SetImageSize(ButtonStyle.Pressed.ImageSize);
	}

	if(bIsMovementButton == false)
	{
		SkillCooldownData = SelectedChar->SkillsCooldownMap.Find(SkillsData.SkillID);
		if(SkillCooldownData)
		{
			SkillCooldownData->RefreshHudOnSkillCastDelegate.Unbind();
			SkillCooldownData->RefreshHudOnSkillCastDelegate.BindDynamic(this, &UBattleCharSkillNodeWidget::OnSkillCasted);
		}
	}
	else
	{
		SkillCooldownData = nullptr;
	}
	

	PlayerController = Cast<ABattlePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	SelectedCharacter = SelectedChar;
	OwnerHud = Hud;

	RefreshNodeState();
}

void UBattleCharSkillNodeWidget::OnSkillCasted()
{
	RefreshNodeState();
}

void UBattleCharSkillNodeWidget::OnRoundEnds()
{
	RefreshNodeState();
}
