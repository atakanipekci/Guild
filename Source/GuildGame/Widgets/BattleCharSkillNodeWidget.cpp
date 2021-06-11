// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharSkillNodeWidget.h"

#include "CharacterSkillTooltipWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GuildGame/Battle/BattlePlayerController.h"
#include "Kismet/GameplayStatics.h"

void UBattleCharSkillNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(SkillButton)
	{
		SkillButton->OnClicked.AddUniqueDynamic(this, &UBattleCharSkillNodeWidget::OnPressed);
		ButtonStyle = SkillButton->WidgetStyle;
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
	if(SelectedCharacter)
	{
		SelectedCharacter->SetCurrentSkillIfContains(SkillsData.SkillID);

		if(PlayerController)
			PlayerController->ChangeStateTo(EControllerStateIndex::SkillCast);

		if(PortraitBg)
		{
			PortraitBg->SetBrushTintColor(CooldownButtonBrushColor);
		}
	}
}

void UBattleCharSkillNodeWidget::RefreshNode(AGGCharacter* SelectedChar)
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

	PlayerController = Cast<ABattlePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	SelectedCharacter = SelectedChar;
}
