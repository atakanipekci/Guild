// Fill out your copyright notice in the Description page of Project Settings.


#include "TownDefaultPawn.h"



#include "LevelSequenceActor.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATownDefaultPawn::ATownDefaultPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

ATownDefaultPawn::ATownDefaultPawn(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	if(SpringArmComponent)
		SpringArmComponent->SetupAttachment(RootComponent);
	if(Camera)
		Camera->SetupAttachment(SpringArmComponent);

}

