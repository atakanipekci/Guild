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

	UPROPERTY()
	TArray<class UCharacterSkillNodeWidget*> SkillNodes;

	

	void RefreshSkillsArray();

	public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UImage*> Lines;
	
	
	void RefreshPage(struct FCharacterStats* Stat);
	void ReleaseSkillNodeButtons(UCharacterSkillNodeWidget* Excluded);
	
};
