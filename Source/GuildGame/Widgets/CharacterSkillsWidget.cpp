// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillsWidget.h"

#include "CharacterSkillLineWidget.h"
#include "CharacterSkillNodeWidget.h"
#include "CharacterSkillTooltipWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GuildGame/Characters/CharacterStats.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "GuildGame/Skills/CharacterSkill.h"

void UCharacterSkillsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(AcquireSkillButton)
	{
		AcquireSkillButton->OnClicked.AddUniqueDynamic(this, &UCharacterSkillsWidget::AcquireSkill);
		AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);
	}

	Tooltip = CreateWidget<UCharacterSkillTooltipWidget>(GetWorld(), WidgetManager::GetWidget(EWidgetKeys::CharacterSkillTooltip));
	//RefreshSkillsArray();

}

void UCharacterSkillsWidget::AcquireSkill()
{
	if(SelectedSkillNode && CharacterStat && AcquireSkillButton && CostText)
	{
		if(CanSkillBeAcquired(SelectedSkillNode))
		{
			CharacterStat->SpendableSkillPoints -= SelectedSkillNode->UnlockSkillPointCost;
			CharacterStat->SkillIDs.Add(SelectedSkillNode->SkillID);
			SelectedSkillNode->SetSkillNodeState(ESkillNodeState::Unlocked);

			CostText->SetVisibility(ESlateVisibility::Hidden);
			AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);

			if(CostTitleText)
				CostTitleText->SetVisibility(ESlateVisibility::Hidden);

			RefreshSkillNodes();
		}
	}
}

void UCharacterSkillsWidget::RefreshSkillsArray()
{
    SelectedSkillNode = nullptr;
	if(AcquireSkillButton)
	{
		AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if(CostText)
		CostText->SetVisibility(ESlateVisibility::Hidden);

	if(CostTitleText)
		CostTitleText->SetVisibility(ESlateVisibility::Hidden);
	
    if(SkillsPanel)
	{

    	if(LinesPanel)
		{
			Lines.Empty();
			for (int i = 0; i < LinesPanel->GetChildrenCount(); ++i)
			{
				
				UCharacterSkillLineWidget* ChildWidget = Cast<UCharacterSkillLineWidget>(LinesPanel->GetChildAt(i));
				if(ChildWidget)
				{
					Lines.Add(ChildWidget);
				}
			}
		}
    	
        SkillNodes.Empty();
    	SkillsMap.Empty();
    	SkillsImageMap.Empty();
    	DescMap.Empty();
    	
    	UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	    if(GameInstance == nullptr || GameInstance->CharacterSkillsFileTable == nullptr)
	    {
	        return;
	    }
    	
		for (int i = 0; i < SkillsPanel->GetChildrenCount(); ++i)
		{
			
			UCharacterSkillNodeWidget* ChildWidget = Cast<UCharacterSkillNodeWidget>(SkillsPanel->GetChildAt(i));
			if(ChildWidget && GameInstance)
			{
				
    	 		FSkillData* SkillData = GameInstance->CharacterSkillsTable->FindRow<FSkillData>(FName(FString::FromInt(ChildWidget->SkillID)),"Skill File Row Missing", true);

				if(SkillData)
					SkillsMap.Add(ChildWidget->SkillID, new FSkillData(*SkillData));


				FCharSkillFileDataTable* SkillFile = GameInstance->CharacterSkillsFileTable->FindRow<FCharSkillFileDataTable>(FName(FString::FromInt(ChildWidget->SkillID)),"Skill File Row Missing", true);

				if(SkillFile)
					SkillsImageMap.Add(ChildWidget->SkillID, SkillFile->SkillImage);

				if(SkillFile)
					DescMap.Add(ChildWidget->SkillID, &SkillFile->Desc);
				
				
				ChildWidget->Lines.Empty();
				for (int j = 0; j < Lines.Num(); ++j)
				{
					if(Lines[j])
					{
						if(Lines[j]->OwnerSkillID == ChildWidget->SkillID)
						{
							ChildWidget->Lines.Add(Lines[j]);
						}
					}
				}
				SkillNodes.Add(ChildWidget);
				ChildWidget->SetToolTip(Tooltip);
				// ChildWidget->SetCursor(EMouseCursor::None);
				ChildWidget->OwnerWidget = this;
				ChildWidget->bIsPressed = false;
				if(ChildWidget->Portrait)
				{
					FCharSkillFileDataTable* SkillFiles = CharacterSkill::GetSkillFiles(ChildWidget->SkillID, GetWorld());
					if(SkillFiles)
					{
						const FVector2D ImageSize = ChildWidget->Portrait->Brush.GetImageSize();
						ChildWidget->Portrait->SetBrushResourceObject(SkillFiles->SkillImage);
						ChildWidget->Portrait->SetBrushSize(ImageSize);
						
					}
				}
			}
		}

    	RefreshSkillNodes();
	}
}

void UCharacterSkillsWidget::RefreshSkillNodes()
{
	if(SkillPoints && CharacterStat && AcquireSkillButton)
	{
		SkillPoints->SetText(FText::AsNumber(CharacterStat->SpendableSkillPoints));

		if(CharacterStat->bIsOwned == false)
		{
			AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	for (int i = 0; i < SkillNodes.Num(); ++i)
	{
		UCharacterSkillNodeWidget* ChildWidget = Cast<UCharacterSkillNodeWidget>(SkillNodes[i]);
		if(ChildWidget)
		{
			if(CharacterStat)
			{
				ChildWidget->ResetButtonStyle(true, true, true);
				if(CharacterStat->bIsOwned == true)
				{
					if(CharacterStat->SkillIDs.Contains(ChildWidget->SkillID))
					{
						ChildWidget->SetSkillNodeState(ESkillNodeState::Unlocked);
					}
					else 
					{
						bool bMeetsConditions = true;
						for (int j = 0; j < ChildWidget->PrerequisitesSkillIDs.Num(); ++j)
						{
							if(CharacterStat->SkillIDs.Contains(ChildWidget->PrerequisitesSkillIDs[j]) == false)
							{
								bMeetsConditions = false;
								break;
							}
						}

						if(bMeetsConditions == true)
						{
							bMeetsConditions = CanSkillBeAcquired(ChildWidget);
						}

						if(bMeetsConditions == false)
						{
							ChildWidget->SetSkillNodeState(ESkillNodeState::Locked);
						}
						else
						{
							ChildWidget->SetSkillNodeState(ESkillNodeState::CanBeUnlocked);
						}
					}
				}
				else
				{
					ChildWidget->SetSkillNodeState(ESkillNodeState::Locked);
				}
			}
		}
	}
}

void UCharacterSkillsWidget::RefreshPage(FCharacterStats* Stat)
{
   if(Stat)
   {
       CharacterStat = Stat;
   	   RefreshSkillsArray();
   }
}

void UCharacterSkillsWidget::ReleaseSkillNodeButtons(UCharacterSkillNodeWidget* Excluded)
{
	if(Excluded && AcquireSkillButton && CostText)
	{
		const int Cost = Excluded->UnlockSkillPointCost;
		// FText CostLocalizedText = NSLOCTEXT("CommonWords", "Cost:", "Cost: {Cost}");
		// FFormatNamedArguments Args;
		// Args.Add("Cost", Cost);
		//
		// FText FormattedText = FText::Format(
		// 	CostLocalizedText,
		// 	Args
		// );
		
		if(Excluded->NodeState == ESkillNodeState::CanBeUnlocked)
		{
			AcquireSkillButton->SetVisibility(ESlateVisibility::Visible);
			AcquireSkillButton->SetIsEnabled(true);
			CostText->SetVisibility(ESlateVisibility::Visible);
			CostText->SetText(FText::AsNumber(Cost));

			if(CostTitleText)
				CostTitleText->SetVisibility(ESlateVisibility::Visible);
		}
		else if(Excluded->NodeState == ESkillNodeState::Unlocked)
		{
			AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);
			AcquireSkillButton->SetIsEnabled(true);
			CostText->SetVisibility(ESlateVisibility::Hidden);

			if(CostTitleText)
				CostTitleText->SetVisibility(ESlateVisibility::Hidden);
		}
		else if(Excluded->NodeState == ESkillNodeState::Locked)
		{
			AcquireSkillButton->SetVisibility(ESlateVisibility::Visible);
			AcquireSkillButton->SetIsEnabled(false);
			CostText->SetVisibility(ESlateVisibility::Visible);
			CostText->SetText(FText::AsNumber(Cost));

			if(CostTitleText)
				CostTitleText->SetVisibility(ESlateVisibility::Visible);
		}
		
		if(CharacterStat && CharacterStat->bIsOwned == false)
		{
			AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);
		}

		
	}
	for (int i = 0; i < SkillNodes.Num(); ++i)
	{
		UCharacterSkillNodeWidget* ChildWidget = SkillNodes[i];
		if(ChildWidget)
		{
			if(ChildWidget != Excluded)
			{
				ChildWidget->ReleaseButton();
			}
		}
	}
}

bool UCharacterSkillsWidget::CanSkillBeAcquired(UCharacterSkillNodeWidget* SkillNode)
{
	if(SkillNode && CharacterStat)
	{
		if(CharacterStat->SkillIDs.Contains(SkillNode->SkillID) == false)
		{
			if(CharacterStat->SpendableSkillPoints > 0 && CharacterStat->SpendableSkillPoints >= SkillNode->UnlockSkillPointCost)
			{
				return true;
			}
		}
	}
	
	return false;
}

void UCharacterSkillsWidget::RefreshTooltip(int SkillID)
{
	if(Tooltip)
	{
		FSkillData** Skill = SkillsMap.Find(SkillID);
		UTexture** SkillImage = SkillsImageMap.Find(SkillID);
		FText** Desc = DescMap.Find(SkillID);
		if(Skill && SkillImage && Desc)
		{
			if(*Skill)
			{
				Tooltip->Refresh(**Skill, *SkillImage, *Desc);
			}
		}
	}
}
