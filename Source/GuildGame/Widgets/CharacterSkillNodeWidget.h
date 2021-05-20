// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSkillNodeWidget.generated.h"

UENUM(Blueprintable)
enum class ECharacterSkillTypes : uint8
{
	FireBall,
	LightningBall,
	Heal
}; 
UCLASS()
class GUILDGAME_API UCharacterSkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void OnPressed();
	
	UPROPERTY(EditAnywhere)
	TArray<ECharacterSkillTypes> Prerequisites;

	UPROPERTY(EditAnywhere)
	ECharacterSkillTypes SkillType;

	UPROPERTY(meta = (BindWidget))
	class UButton* SkillButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* Portrait;

	FButtonStyle LatestStyle;

	UPROPERTY(EditAnywhere)
	int UnlockSkillPointCost;

	public:

	bool bIsPressed;

	UPROPERTY()
	class UCharacterSkillsWidget* OwnerWidget;

	void ReleaseButton();


};
