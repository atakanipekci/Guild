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

	UPROPERTY()
	TArray<class UCharacterSkillNodeWidget*> SkillNodes;

	UPROPERTY()
	TArray<class UCharacterSkillLineWidget*> Lines;

	UFUNCTION()
	void AcquireSkill();
	

	void RefreshSkillsArray();
	void RefreshSkillNodes();

	public:

	UPROPERTY(EditAnywhere)
	FSlateColor CanBeUnlockedButtonColor;

	UPROPERTY()
	class UCharacterSkillNodeWidget* SelectedSkillNode;
	
	
	
	
	void RefreshPage(struct FCharacterStats* Stat);
	void ReleaseSkillNodeButtons(UCharacterSkillNodeWidget* Excluded);
	bool CanSkillBeAcquired(class UCharacterSkillNodeWidget* SkillNode);
	
};
