// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridStatusEffectVfxActor.generated.h"

UCLASS()
class GUILDGAME_API AGridStatusEffectVfxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridStatusEffectVfxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystemComponent* ParticleSystem;

public:	
	// Called every frame
	void StopParticleEffectAndDestroy();
	virtual void Tick(float DeltaTime) override;

};
