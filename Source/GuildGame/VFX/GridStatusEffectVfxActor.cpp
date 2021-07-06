// Fill out your copyright notice in the Description page of Project Settings.


#include "GridStatusEffectVfxActor.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
AGridStatusEffectVfxActor::AGridStatusEffectVfxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	if(ParticleSystem)
	{
		ParticleSystem->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		// ParticleSystem->AddToRoot();
	}
}

// Called when the game starts or when spawned
void AGridStatusEffectVfxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGridStatusEffectVfxActor::StopParticleEffectAndDestroy()
{
	if(ParticleSystem)
	{
		// ParticleSystem->ActivateSystem(false);
		SetLifeSpan(0.1f);
	}
}

// Called every frame
void AGridStatusEffectVfxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

