// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillNodeWidget.h"

#include "CharacterSkillsWidget.h"
#include "Components/Button.h"

void UCharacterSkillNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(SkillButton)
	{
		SkillButton->OnClicked.AddUniqueDynamic(this, &UCharacterSkillNodeWidget::OnPressed);
	}

}

void UCharacterSkillNodeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCharacterSkillNodeWidget::OnPressed()
{
	if(SkillButton && bIsPressed == false)
	{
		if(OwnerWidget)
		{
			OwnerWidget->ReleaseSkillNodeButtons(this);
		}
		bIsPressed = true;
		LatestStyle = SkillButton->WidgetStyle;
		SkillButton->WidgetStyle.Normal = LatestStyle.Pressed;
		SkillButton->WidgetStyle.Hovered = LatestStyle.Pressed;
	}
}

void UCharacterSkillNodeWidget::ReleaseButton()
{
	if(bIsPressed == true)
	{
		bIsPressed = false;
		SkillButton->WidgetStyle.Normal = LatestStyle.Normal;
		SkillButton->WidgetStyle.Hovered = LatestStyle.Hovered;
	}
}
