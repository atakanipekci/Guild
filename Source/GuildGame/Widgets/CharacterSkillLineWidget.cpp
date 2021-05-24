// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillLineWidget.h"

#include "Components/Image.h"

void UCharacterSkillLineWidget::SetLineColorAndOpacity(FLinearColor Col)
{
	LineImage->SetColorAndOpacity(Col);
}
