// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "CharacterSkillsWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UCharacterSkillsWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	struct FCharacterStats* CharacterStat;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* SkillsPanel;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* LinesPanel;

	UPROPERTY(meta = (BindWidget))
	class UButton* AcquireSkillButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillPoints;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CostText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CostTitleText;



	UPROPERTY()
	TArray<class UCharacterSkillNodeWidget*> SkillNodes;

	UPROPERTY()
	TArray<class UCharacterSkillLineWidget*> Lines;

	TMap<int, struct FSkillData*> SkillsMap;
	UPROPERTY()
	TMap<int, UTexture*> SkillsImageMap;

	TMap<int, FText*> DescMap;

	UFUNCTION()
	void AcquireSkill();
	

	void RefreshSkillsArray();
	void RefreshSkillNodes();

	public:


	UPROPERTY()
	class UCharacterSkillTooltipWidget* Tooltip;
	
	UPROPERTY()
	class UCharacterSkillNodeWidget* SelectedSkillNode;
	
	void RefreshPage(struct FCharacterStats* Stat);
	void ReleaseSkillNodeButtons(UCharacterSkillNodeWidget* Excluded);
	bool CanSkillBeAcquired(class UCharacterSkillNodeWidget* SkillNode);
	void RefreshTooltip(int SkillID);
	
};
