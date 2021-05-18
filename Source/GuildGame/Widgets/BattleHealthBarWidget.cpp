// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBattleHealthBarWidget::SetHpBar(int CurrentHealth, int MaxHealth)
{
	if(MaxHealth <= 0 || HealthBar == nullptr || HpText == nullptr)
		return;
	
	const float Percentage = (float)CurrentHealth / MaxHealth;

	HealthBar->SetPercent(Percentage);
	FString Hp = FString::Printf(TEXT("%d"), CurrentHealth);
	HpText->SetText(FText::FromString(Hp));
}
