// Fill out your copyright notice in the Description page of Project Settings.


#include "TownDefaultPawn.h"



#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "TownBuildingActorComponent.h"
#include "TownBuildingWidgetBase.h"
#include "TownGameModeBase.h"
#include "TownPlayerController.h"
#include "TownYesOrNoWidget.h"
#include "WidgetManager.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/Overlay.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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
/*
void ATownDefaultPawn::LeftClickHandler()
{
	if (PlayerController != nullptr)
	{
		FHitResult Hit;
		PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, Hit);
		if (Hit.bBlockingHit)
		{
			if(Hit.Actor != nullptr)
			{
				UActorComponent* Building = Hit.Actor->GetComponentByClass(UTownBuildingActorComponent::StaticClass());
				if(Building != nullptr)
				{
					if(bIsBuildingFocused == false && bEnableInput == true)
					{
						SelectedBuilding = Cast<UTownBuildingActorComponent>(Building);
						//SequenceAsset = SelectedBuilding->GetSequenceAsset();
						SequenceAsset = WidgetManager::GetSequence(SelectedBuilding->SequenceKey);
						PlaySequence((Hit.Actor));
					}
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("RAYCAST ITEM, %s"),*(Hit.Actor->GetName()));
		}	
	}
}

void ATownDefaultPawn::ZoomOutFromBuilding()
{
	if (PlayerController != nullptr)
	{
		if(bIsBuildingFocused && bEnableInput == true && SequenceAsset != nullptr)
		{
			PlaySequenceReverse();
			
			if(UTownYesOrNoWidget::YesOrNoWidgetInstance)
				UTownYesOrNoWidget::YesOrNoWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ATownDefaultPawn::PlaySequence(TWeakObjectPtr<AActor> Actor)
{
	if(SequenceAsset == nullptr)
		return;
	
	if (SequencePlayer == nullptr && SequenceActor == nullptr)
	{
          SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SequenceAsset, FMovieSceneSequencePlaybackSettings(), SequenceActor);
	}
     
    if (SequencePlayer)
    {
    	UE_LOG(LogTemp, Warning, TEXT("SETTING SEQ , %s"),*(SequenceAsset->GetName()));
    	SequenceActor->SetSequence(SequenceAsset);
    	bIsBuildingFocused = true;
    	SequencePlayer->Stop();
        SequencePlayer->Play();
    	bEnableInput = false;
    	SequencePlayer->OnFinished.AddUniqueDynamic(this, &ATownDefaultPawn::EnablePlayerInputOnSequenceFinish);
    }
}

void ATownDefaultPawn::PlaySequenceReverse()
{
	if (SequenceAsset && SequencePlayer && SequenceActor)
	{
	   SequenceActor->LevelSequence = SequenceAsset;
       bIsBuildingFocused = false;
	   SequencePlayer->Stop();
       SequencePlayer->PlayReverse();
	   bEnableInput = false;
	   SequencePlayer->OnFinished.AddUniqueDynamic(this, &ATownDefaultPawn::EnablePlayerInputOnSequenceFinish);
	}
}

void ATownDefaultPawn::EnablePlayerInputOnSequenceFinish()
{
	bEnableInput = true;
}

void ATownDefaultPawn::CreateWidgetViaCode()
{
	if(SelectedBuilding)
	{
		UTownBuildingWidgetBase* BuildingWidgetInstance = Cast<UTownBuildingWidgetBase>(WidgetManager::GetWidgetInstance(SelectedBuilding->WidgetKey));
		if(BuildingWidgetInstance)
		{
			if(IsReversed == true)
			{
				IsReversed = false;
				FWidgetAnimationDynamicEvent OnFinishEvent;
				OnFinishEvent.BindDynamic(this, &ATownDefaultPawn::CollapseBuildingWidgetOnAnimationFinish);
				BuildingWidgetInstance->BindToAnimationFinished(BuildingWidgetInstance->CloseDownAnimation, OnFinishEvent);
				UUMGSequencePlayer*  AnimPlayer = BuildingWidgetInstance->PlayAnimation(BuildingWidgetInstance->CloseDownAnimation);
			}
			else
			{
				IsReversed = true;
				BuildingWidgetInstance->OnEnabled();
				BuildingWidgetInstance->SetVisibility(ESlateVisibility::Visible);
				BuildingWidgetInstance->PlayAnimation(BuildingWidgetInstance->OpenUpAnimation);
			}
		}
		else
		{
			//if(BuildingWidgetInstance != nullptr)
			//{
				ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this));
				UTownBuildingWidgetBase* NewWidget = CreateWidget<UTownBuildingWidgetBase>(this->GetWorld(), WidgetManager::GetWidget(SelectedBuilding->WidgetKey));
				//SetMappedWidgetInstance(Key, NewWidget);
				WidgetManager::SetWidgetInstance(SelectedBuilding->WidgetKey, NewWidget);
				if(GameMode->MenuUiContainerOverlay)
				{
					GameMode->MenuUiContainerOverlay->AddChildToOverlay(NewWidget);
				}
				IsReversed = true;
			//}
		}
	}
}


void ATownDefaultPawn::CollapseBuildingWidgetOnAnimationFinish()
{
	if(SelectedBuilding)
	{
		UTownBuildingWidgetBase* BuildingWidgetInstance = Cast<UTownBuildingWidgetBase>(WidgetManager::GetWidgetInstance(SelectedBuilding->WidgetKey));
		if(BuildingWidgetInstance)
		{
			BuildingWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// Called when the game starts or when spawned
void ATownDefaultPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ATownPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

// Called every frame
void ATownDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	return;
	
	if(PlayerController == nullptr)
		return;
	
	FHitResult Hit;
	PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, Hit);
	if (Hit.bBlockingHit)
	{
		if(Hit.Actor != nullptr && bIsBuildingFocused == false)
		{
			UActorComponent* Building = Hit.Actor->GetComponentByClass(UTownBuildingActorComponent::StaticClass());
			if(Building != nullptr)
			{
				UActorComponent* StaticMesh = Hit.Actor->GetComponentByClass(UStaticMeshComponent::StaticClass());
				if(StaticMesh)
				{
					UStaticMeshComponent* NewStaticMesh = Cast<UStaticMeshComponent>(StaticMesh);
					if(HoveredBuildingStaticMeshComp != nullptr)
					{
						HoveredBuildingStaticMeshComp->SetRenderCustomDepth(false);
					}
					HoveredBuildingStaticMeshComp = NewStaticMesh;
					HoveredBuildingStaticMeshComp->SetRenderCustomDepth(true);
					return;
				}
			}
		}
	}
	
	if(HoveredBuildingStaticMeshComp)
	{
		HoveredBuildingStaticMeshComp->SetRenderCustomDepth(false);
		HoveredBuildingStaticMeshComp = nullptr;
	}

}

// Called to bind functionality to input
void ATownDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	return;
	if (!PlayerInputComponent)
	{
		return;
	}
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent->BindAction("ZoomOutByWheel", IE_Pressed, this, &AMyDefaultPawn::ZoomOutByWheel);
	// PlayerInputComponent->BindAction("ZoomInByWheel", IE_Pressed, this, &AMyDefaultPawn::ZoomInByWheel);
	// PlayerInputComponent->BindAction("RotateUpByWheel", IE_Pressed, this, &AMyDefaultPawn::RotateUpByWheel);
	// PlayerInputComponent->BindAction("RotateDownByWheel", IE_Pressed, this, &AMyDefaultPawn::RotateDownByWheel);
	// PlayerInputComponent->BindAction("RotateLeftByWheel", IE_Pressed, this, &AMyDefaultPawn::RotateLeftByWheel);
	// PlayerInputComponent->BindAction("RotateRightByWheel", IE_Pressed, this, &AMyDefaultPawn::RotateRightByWheel);
	// PlayerInputComponent->BindAxis("MoveForward", this, &AMyDefaultPawn::MoveCameraForwardInput);
	// PlayerInputComponent->BindAxis("MoveRight", this, &AMyDefaultPawn::MoveCameraRightInput);
	// PlayerInputComponent->BindAxis("MoveUp", this, &AMyDefaultPawn::MoveCameraUpInput);
	// PlayerInputComponent->BindAxis("ZoomIn", this, &AMyDefaultPawn::ZoomCameraInInput);
	// PlayerInputComponent->BindAxis("Rotate", this, &AMyDefaultPawn::RotateInput);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATownDefaultPawn::LeftClickHandler);
	//PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATownDefaultPawn::CreateWidgetViaCode);
	//PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &AMyDefaultPawn::LeftClickReleaseHandler);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, this, &ATownDefaultPawn::ZoomOutFromBuilding);

	// PlayerInputComponent->BindAction("RightMouseClick", IE_Released, this, &AMyDefaultPawn::RightClickReleaseHandler);

}
*/
