// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnInfoNodeWidget.h"

#include "Components/TextBlock.h"

void UTurnInfoNodeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// if(Text)
	// {
	// 	Text->SetText(FText::AsNumber(Index));
	// }
	// if(LeftText)
	// {
	// 	if(LeftNode)
	// 	{
	// 		LeftText->SetText(FText::AsNumber(LeftNode->Index));
	// 	}
	// 	else
	// 	{
	// 		FString NullText = "Null";
	// 		LeftText->SetText(FText::FromString(NullText));
	// 	}
	// }
	// if(RightText)
	// {
	// 	if(RightNode)
	// 	{
	// 		RightText->SetText(FText::AsNumber(RightNode->Index));
	// 	}
	// 	else
	// 	{
	// 		FString NullText = "Null";
	// 		RightText->SetText(FText::FromString(NullText));
	// 	}
	// }
}

void UTurnInfoNodeWidget::EmptyData()
{
	Character = nullptr;
	LeftNode = nullptr;
	RightNode = nullptr;
	
	IsRemoved = false;
	IsSeperator = false;
	Index = -1;
	GridIndex = -1;

	IndexFirst = -1;
	IndexSecond = -1;
	SwapOffSet = 0;

}
