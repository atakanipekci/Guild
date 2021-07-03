// Fill out your copyright notice in the Description page of Project Settings.


#include "ApNodeWidget.h"

#include "Components/Image.h"

void UApNodeWidget::SetFull()
{
	if(FilledImage && BlankImage)
	{
		// BlankImage->SetVisibility(ESlateVisibility::Collapsed);
		FilledImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UApNodeWidget::SetBlank()
{
	if(FilledImage && BlankImage)
	{
		BlankImage->SetVisibility(ESlateVisibility::Visible);
		FilledImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}
