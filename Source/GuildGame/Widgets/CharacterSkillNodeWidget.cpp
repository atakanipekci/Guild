// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillNodeWidget.h"

#include "CharacterSkillLineWidget.h"
#include "CharacterSkillsWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UCharacterSkillNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	if(bIsConstructorCalledOnce == false && SkillButton)
	{
		SkillButton->OnClicked.AddUniqueDynamic(this, &UCharacterSkillNodeWidget::OnPressed);
		ButtonStyle = SkillButton->WidgetStyle;
	}

	bIsConstructorCalledOnce = true;

}

void UCharacterSkillNodeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCharacterSkillNodeWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(OwnerWidget)
	{
		OwnerWidget->RefreshTooltip(SkillID);
	}
}

void UCharacterSkillNodeWidget::OnPressed()
{
	if(SkillButton && bIsPressed == false)
	{
		if(OwnerWidget)
		{
			OwnerWidget->ReleaseSkillNodeButtons(this);
			OwnerWidget->SelectedSkillNode = this;
		}
		bIsPressed = true;
		SkillButton->WidgetStyle.SetNormal(ButtonStyle.Pressed);
		SkillButton->WidgetStyle.SetHovered(ButtonStyle.Pressed);
	}
}

void UCharacterSkillNodeWidget::ReleaseButton()
{
	if(SkillButton && bIsPressed == true)
	{
		bIsPressed = false;
		ResetButtonStyle(true, true, false);
		SetSkillNodeState(NodeState);
	}
}

void UCharacterSkillNodeWidget::ResetButtonStyle(bool Normal, bool Hover, bool Pressed)
{
	if(SkillButton == nullptr) return;

	if(Normal)
		SkillButton->WidgetStyle.SetNormal(ButtonStyle.Normal);

	if(Hover)
		SkillButton->WidgetStyle.SetHovered(ButtonStyle.Hovered);

	if(Pressed)
		SkillButton->WidgetStyle.SetPressed(ButtonStyle.Pressed);
}

void UCharacterSkillNodeWidget::SetSkillNodeState(ESkillNodeState State)
{
	if(Portrait == nullptr || SkillButton == nullptr || OwnerWidget == nullptr) return;

	NodeState = State;

	if(State == ESkillNodeState::Locked)
	{
		Portrait->SetOpacity(0.5f);
		
		ResetButtonStyle(true, false, false);

		FLinearColor LineColor = ButtonStyle.Normal.GetTint(FWidgetStyle());
		LineColor.A = 0.5f;
		for (int i = 0; i < Lines.Num(); ++i)
		{
			if(Lines[i])
			{
				Lines[i]->SetLineColorAndOpacity(LineColor);
			}
		}
	}
	else if(State == ESkillNodeState::Unlocked)
	{
		Portrait->SetOpacity(1.0f);

		ResetButtonStyle(true, false, false);

		FLinearColor LineColor = ButtonStyle.Normal.GetTint(FWidgetStyle());
		LineColor.A = 1.0f;
		for (int i = 0; i < Lines.Num(); ++i)
		{
			if(Lines[i])
			{
				Lines[i]->SetLineColorAndOpacity(LineColor);
			}
		}
	}
	else if(State == ESkillNodeState::CanBeUnlocked)
	{
		Portrait->SetOpacity(0.5f);

		SkillButton->WidgetStyle.Normal.SetResourceObject(CanBeUnlockedButtonBrush.GetResourceObject());
		SkillButton->WidgetStyle.Normal.SetImageSize(ButtonStyle.Normal.ImageSize);
		SkillButton->WidgetStyle.Normal.TintColor = CanBeUnlockedButtonBrush.TintColor;

		FLinearColor LineColor = CanBeUnlockedButtonBrush.TintColor.GetColor(FWidgetStyle());
		LineColor.A = 0.5f;
		for (int i = 0; i < Lines.Num(); ++i)
		{
			if(Lines[i])
			{
				Lines[i]->SetLineColorAndOpacity(LineColor);
			}
		}
	}

	if(bIsPressed)
	{
		SkillButton->WidgetStyle.SetNormal(ButtonStyle.Pressed);
		SkillButton->WidgetStyle.SetHovered(ButtonStyle.Pressed);
	}
	
}
