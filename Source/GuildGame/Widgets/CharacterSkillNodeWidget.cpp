// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillNodeWidget.h"

#include "CharacterSkillLineWidget.h"
#include "CharacterSkillsWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UCharacterSkillNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(SkillButton)
	{
		SkillButton->OnClicked.AddUniqueDynamic(this, &UCharacterSkillNodeWidget::OnPressed);
		ButtonStyle = SkillButton->WidgetStyle;
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
			OwnerWidget->SelectedSkillNode = this;
		}
		bIsPressed = true;
		SkillButton->WidgetStyle.Normal = ButtonStyle.Pressed;
		SkillButton->WidgetStyle.Hovered = ButtonStyle.Pressed;
	}
}

void UCharacterSkillNodeWidget::ReleaseButton()
{
	if(SkillButton && bIsPressed == true)
	{
		bIsPressed = false;
		SkillButton->WidgetStyle.Normal = ButtonStyle.Normal;
		SkillButton->WidgetStyle.Hovered = ButtonStyle.Hovered;
		SetSkillNodeState(NodeState);
	}
}

void UCharacterSkillNodeWidget::ResetButtonStyle()
{
	if(SkillButton == nullptr) return;
	
	SkillButton->WidgetStyle.Normal = ButtonStyle.Normal;
	SkillButton->WidgetStyle.Hovered = ButtonStyle.Hovered;
	SkillButton->WidgetStyle.Pressed = ButtonStyle.Pressed;
}

void UCharacterSkillNodeWidget::SetSkillNodeState(ESkillNodeState State)
{
	if(Portrait == nullptr || SkillButton == nullptr || OwnerWidget == nullptr) return;

	NodeState = State;

	if(State == ESkillNodeState::Locked)
	{
		FLinearColor Color = FLinearColor::White;
		Color.A = 0.5f;
		Portrait->SetColorAndOpacity(Color);
		
		SkillButton->WidgetStyle.Normal = ButtonStyle.Normal;

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
		FLinearColor Color = FLinearColor::White;
		Color.A = 1;
		Portrait->SetColorAndOpacity(Color);

		SkillButton->WidgetStyle.Normal = ButtonStyle.Normal;

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
		FLinearColor Color = FLinearColor::White;
		Color.A = 0.5f;
		Portrait->SetColorAndOpacity(Color);

		SkillButton->WidgetStyle.Normal.TintColor = OwnerWidget->CanBeUnlockedButtonColor;

		FLinearColor LineColor = OwnerWidget->CanBeUnlockedButtonColor.GetColor(FWidgetStyle());
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
		SkillButton->WidgetStyle.Normal = ButtonStyle.Pressed;
		SkillButton->WidgetStyle.Hovered = ButtonStyle.Pressed;
	}
	
}
