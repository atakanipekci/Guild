// Fill out your copyright notice in the Description page of Project Settings.


#include "ApBarWidget.h"
#include "ApNodeWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "GuildGame/Managers/TimedEventManager.h"
#include "GuildGame/Managers/WidgetManager.h"

void UApBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Pool.SetWorld(GetWorld());
}

void UApBarWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if(Pool.IsInitialized())
	{
		Pool.ReleaseAllSlateResources();
	}
}

void UApBarWidget::ResetBar(int TotalAp)
{
	if(ApHorizBox == nullptr || EndSpacer == nullptr) return;

	StopPreview();

	for (int i = ApHorizBox->GetChildrenCount() - 1; i >= 0; --i)
	{
		UApNodeWidget* Child = Cast<UApNodeWidget>(ApHorizBox->GetChildAt(i));
		if(Child)
		{
			if(Pool.IsInitialized())
			{
				Pool.Release(Child);
			}
			ApHorizBox->RemoveChildAt(i);
		}
	}
	
	for (int i = 0; i < TotalAp; ++i)
	{
		if(Pool.IsInitialized())
		{
			UApNodeWidget* NewNode = Cast<UApNodeWidget>(Pool.GetOrCreateInstance(AWidgetManager::GetWidget(EWidgetKeys::ApNode, GetWorld())));
			if(NewNode)
			{
				NewNode->SetFull();
				ApHorizBox->AddChildToHorizontalBox(NewNode);
			}
		}
	}
	FullAmount = TotalAp;
}

void UApBarWidget::RefreshBar(int CurrentAp)
{
	if(ApHorizBox == nullptr) return;

	FullAmount = CurrentAp;

	StopPreview();
	for (int i = 0; i < ApHorizBox->GetChildrenCount(); ++i)
	{
		UApNodeWidget* Child = Cast<UApNodeWidget>(ApHorizBox->GetChildAt(i));
		if(Child)
		{
			if(0 < CurrentAp)
			{
				Child->SetFull();
				CurrentAp--;
			}
			else
			{
				Child->SetBlank();
			}
		}
	}
}

void UApBarWidget::PreviewSpending(int AmountToPreview)
{
	if(ApHorizBox == nullptr) return;

	for (int i = FullAmount - 1; i >= 0 && i < ApHorizBox->GetChildrenCount(); --i)
	{
		UApNodeWidget* Child = Cast<UApNodeWidget>(ApHorizBox->GetChildAt(i));
		if(Child)
		{
			if(0 < AmountToPreview)
			{
				ATimedEventManager::LerpWidgetOpacity(Child, 0.4f, 1, 0, true, GetWorld());
				AmountToPreview--;
			}
			else
			{
				ATimedEventManager::RemoveWidgetOpacityTimer(Child);
				Child->SetRenderOpacity(1);
			}
		}
	}
}
void UApBarWidget::StopPreview()
{
	if(ApHorizBox == nullptr) return;

	for (int i = ApHorizBox->GetChildrenCount() - 1; i >= 0; --i)
	{
		UApNodeWidget* Child = Cast<UApNodeWidget>(ApHorizBox->GetChildAt(i));
		if(Child)
		{
			ATimedEventManager::RemoveWidgetOpacityTimer(Child);
			Child->SetRenderOpacity(1);
		}
	}
}
